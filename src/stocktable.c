#include "stocktable.h"
#include <string.h>

#define FNV_OFFSET_BASIS 2166136261
#define FNV_PRIME 16777619

/*
 * Initalizes a StockEntry struct
 */
static void stockEntryNew(struct StockEntry* entry){
		entry->stock = NULL;
		entry->price = 0;
		entry->sharesOwned = 0;
		entry->next = NULL;
}

/*
 * Initalizes a StockTable struct
 */
bool stockTableNew(struct StockTable* table, size_t approxSize){
	size_t realSize = highestOrderBit(approxSize) << 1;

	table->size = realSize;
	table->bitMask = realSize - 1; // used when generating hash for quick modulo
	table->entries = malloc(sizeof(struct StockEntry) * realSize);
	
	if (table->entries != NULL) {
		for (unsigned int i = 0; i < realSize; i++){
			struct StockEntry* entry = table->entries + i;
			stockEntryNew(entry);
		}
		return true;
	}
	return false;
}

/*
 * Sets price per share for a particular stock
 */
bool stockTableSetPrice(struct StockTable* table, char* key, float value){
	uint32_t index = stockTableHash(key, strlen(key), table->bitMask);
	struct StockEntry* entry = table->entries + index;

	// only search through list if first slot isnt open
	if (entry->stock != NULL){

		while (entry != NULL){
			if (strcmp(key, entry->stock) == 0) {
				break;
			}
			entry = entry->next;
		}

		// if entry is NULL, that means we need to make a new slot.
		if (entry == NULL){
			entry = malloc(sizeof(struct StockEntry));

			if (entry == NULL) return false;
			stockEntryNew(entry);
		}

	}

	entry->stock = key;
	entry->price = value;
	return true;	
}

/*
 * Finds a StockEntry based on the stock name specified.
 */
struct StockEntry* stockTableGetEntry(struct StockTable* table, char* key){
	uint32_t index = stockTableHash(key, strlen(key), table->bitMask);
	struct StockEntry* entry = table->entries + index;

	if (entry->stock != NULL){
		while (entry != NULL){
			if (strcmp(key, entry->stock) == 0) {
				return entry;
			}
			entry = entry->next;
		}
	}
	return NULL;
}

/*
 * The following hash function uses FNV-1a.
 *
 * The algo is public domain:
 * http://www.isthe.com/chongo/tech/comp/fnv/
 */
uint32_t stockTableHash(char* key, size_t strLen, size_t bitMask) {
	uint32_t hash = FNV_OFFSET_BASIS;
	for (int i = 0; i < strLen; i++) {
		hash ^= *(key + i); // get char in index i in string
		hash *= FNV_PRIME;  
	}
	return hash & bitMask;
}

/* http://stackoverflow.com/questions/53161/find-the-highest-order-bit-in-c
 * Using this for quick modulo, as seen in: http://codereview.stackexchange.com/questions/73542/hashtable-implementation
 */
int highestOrderBit(int n) {
	n |= (n >>  1);
    n |= (n >>  2);
    n |= (n >>  4);
    n |= (n >>  8);
    n |= (n >> 16);
    return n ^ (n >> 1);
}
