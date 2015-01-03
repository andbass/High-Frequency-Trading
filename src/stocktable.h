#ifndef MARKET_TABLE_H
#define MARKET_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* A linked list meant to be stored within a StockTable. */
struct StockEntry {
	char* stock;

	double price;
	int sharesOwned;

	struct StockEntry* next;
};

/*
 * Initalizes a StockEntry
 */
void stockEntryNew(struct StockEntry* entry);

/*
 * Frees allocated memory within a stockEntry, namely the stock name string
 * Assumes char* points to allocated memory
 */
void stockEntryFree(struct StockEntry* entry);

/*
 * Basically a hashtable geared towards storing stock info.
 */
struct StockTable {
	struct StockEntry* entries;
	size_t size; // Larger size means less chance of collisons, so make this baby large!!!
	size_t bitMask; // this is ANDed with hash.

	struct StockEntry* trackedPairs[4096]; // Storing keys seperately allows for quick iteration through all of the key value pairs in the table, especially with a large table size
	int index; // Cur index within keys array
};

/*
 * Creates a new StockTable
 */
bool stockTableNew(struct StockTable* table, size_t approxSize);

/*
 * Sets a stock's price.  
 */
bool stockTableSetPrice(struct StockTable* table, char* key, double value);

/*
 * Gets information about stock, if it is stored in the StockTable.
 * Returns NULL if the key could not be matched
 */
struct StockEntry* stockTableGetEntry(struct StockTable* table, char* key);

/*
 * Calculates hash value for a key
 */
uint32_t stockTableHash(char* key, size_t bitMask);

/*
 * Frees allocated memory in a stockTable.
 *
 * If the table struct itself was allocated in the heap,
 * free must be called on table aswell to fully deallocate the table.
 */
void stockTableFree(struct StockTable* table);

/*
 * Outputs a stock table and budget info to a file, nicely formatted
 */
void stockTableDump(struct StockTable* table, double originalBudget, double finalBudget, double threshold, FILE* file);

/*
 * Will return the MSB of a binary number
 */
int highestOrderBit(int n);

#endif
