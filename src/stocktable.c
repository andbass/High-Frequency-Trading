#include "stocktable.h"
#include <string.h>

#define FNV_OFFSET_BASIS 2166136261
#define FNV_PRIME 16777619

#define LINE_SEPERATOR "===================================\n"

/*
 * Initalizes a StockEntry struct
 */
void stockEntryNew(struct StockEntry* entry){
		entry->stock = NULL;
		entry->price = 0;
		entry->sharesOwned = 0;
		entry->next = NULL;
}

/*
 * Frees allocated memory within StockEntry struct
 */
void stockEntryFree(struct StockEntry* entry){
	while(entry != NULL){
		free(entry->stock);
		entry = entry->next;
		free(entry);
	}
}

/*
 * Initalizes a StockTable struct
 */
bool stockTableNew(struct StockTable* table, size_t approxSize){
	size_t realSize = highestOrderBit(approxSize) << 1;

	table->size = realSize;
	table->bitMask = realSize - 1; // used when generating hash for quick modulo
	table->entries = malloc(sizeof(struct StockEntry) * realSize);
	table->index = 0;

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
bool stockTableSetPrice(struct StockTable* table, char* key, double value){
	uint32_t index = stockTableHash(key, table->bitMask);
	struct StockEntry* entry = table->entries + index;

	bool isSameKey = false;

	// only search through list if first slot isnt open
	if (entry->stock != NULL){

		while (entry != NULL){
			if (strcmp(key, entry->stock) == 0) {
				isSameKey = true;
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
	
	if (!isSameKey){
		free(entry->stock);
		entry->stock = malloc(sizeof(char) * strlen(key));
		strcpy(entry->stock, key);

		table->keys[table->index++] = &entry->stock;
	}	

	entry->price = value;
	return true;	
}

/*
 * Finds a StockEntry based on the stock name specified.
 */
struct StockEntry* stockTableGetEntry(struct StockTable* table, char* key){
	uint32_t index = stockTableHash(key, table->bitMask);
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

void stockTableDump(struct StockTable* table, double originalBudget, double finalBudget, double threshold, FILE* file){
	fputs(LINE_SEPERATOR, file);
	fprintf(file, "= Starting Budget:\t%.0lf\n", originalBudget);
	fputs(LINE_SEPERATOR, file);

	fprintf(file, "= Ending Budget:\t%.0lf\n", finalBudget);
	fputs(LINE_SEPERATOR, file);

	fprintf(file, "= Threshold:\t\t%.0lf\n", threshold);
	fputs(LINE_SEPERATOR, file);
	
	fputs("=\tSymbol\t=\t# of Shares\n", file);	
	fputs(LINE_SEPERATOR, file);

	for (int i = 0; i < table->index; i++){
		char** keyLocation = table->keys[i]; // why store a pointer to a char pointer you may ask?	
		int sharesOwned = *(int*)( (double*)(keyLocation + 1) + 1); // SUPER FAST POINTER MAGIC!!!
		
		fprintf(file, "=\t%s\t=\t%d\n", *keyLocation, sharesOwned);
		fputs(LINE_SEPERATOR, file);	
	}		
}

/*
 * The following hash function uses FNV-1a.
 *
 * The algo is public domain:
 * http://www.isthe.com/chongo/tech/comp/fnv/
 */
uint32_t stockTableHash(char* key, size_t bitMask) {
	uint32_t hash = FNV_OFFSET_BASIS;
	
	int i = 0;
	while (key[i] != '\0') {
		hash ^= key[i]; // get char in index i in string
		hash *= FNV_PRIME;  
		++i;
	}
	return hash & bitMask;
}

/*
 * Frees memory allocated in the creation of a StockTable
 */
void stockTableFree(struct StockTable* table) {
	for(unsigned int i = 0; i < table->size; i++){
		stockEntryFree(table->entries + i);
	}
	free(table->entries);
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
