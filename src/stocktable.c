#include "stocktable.h"
#include <string.h>

#define FNV_OFFSET_BASIS 2166136261
#define FNV_PRIME 16777619

bool stockTableNew(struct StockTable* table, size_t approxSize){
	size_t realSize = highestOrderBit(approxSize) << 1;

	table->size = realSize;
	table->bitMask = realSize - 1; // used when generating hash for quick modulo
	table->entries = malloc(sizeof(struct StockEntry) * realSize);
	
	if (table->entries != NULL) {
		for (unsigned int i = 0; i < realSize; i++){
			struct StockEntry* entry = table->entries + i;
			entry->stock = NULL;
			entry->price = 0;
			entry->next = NULL;
		}
		return true;
	}
	return false;
}

bool stockTableSet(struct StockTable* table, char* key, float value){
	uint32_t index = stockTableHash(key, strlen(key), table->bitMask);
	struct StockEntry* entry = table->entries + index;

	/* either get the entry with the same key value or get the next empty slot
	 * only need to do this if the first slot is occupied */
	if (entry->stock != NULL){
		while (entry != NULL){
			if (strcmp(key, entry->stock) == 0) {
				break;
			}
			entry = entry->next;
		}
	}

	// if entry is NULL, that means we need to make a new slot.
	if (entry == NULL){
		entry = malloc(sizeof(struct StockEntry));
		if (entry == NULL) return false;

		entry->stock = NULL;
		entry->price = 0;
		entry->next = NULL;
	}

	entry->stock = key;
	entry->price = value;
	return true;	
}

bool stockTableGet(struct StockTable* table, char* key, float* out){
	uint32_t index = stockTableHash(key, strlen(key), table->bitMask);
	struct StockEntry* entry = table->entries + index;

	while (entry != NULL){
		if (strcmp(key, entry->stock) == 0) {
			*out = entry->price;
			return true;
		}
	}
	return false;
}

/*
 * The following hash function uses FNV-1a.
 *
 * The algo is public domain:
 * http://www.isthe.com/chongo/tech/comp/fnv/
 */
uint32_t stockTableHash(char* key, size_t strLen, size_t bitMask){
	uint32_t hash = FNV_OFFSET_BASIS;
	for (int i = 0; i < strLen; i++){ // magic!
		hash ^= *(key + i); // get char in index i in string
		hash *= FNV_PRIME;  
	}
	return hash & bitMask;
}

/* http://stackoverflow.com/questions/53161/find-the-highest-order-bit-in-c
 * Using this for quick modulo, as seen in: http://codereview.stackexchange.com/questions/73542/hashtable-implementation
 */
int highestOrderBit(int n){
	n |= (n >>  1);
    n |= (n >>  2);
    n |= (n >>  4);
    n |= (n >>  8);
    n |= (n >> 16);
    return n ^ (n >> 1);
}
