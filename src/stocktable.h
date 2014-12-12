#ifndef MARKET_TABLE_H
#define MARKET_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* A linked list meant to be stored within a StockTable.
 * Basically a hashtable entry, where a pointer to the next entry
 * for a particular index is stored to accomdate collisons
 *
 * I should really switch it to a dynamically sized array
 * later on, but a linked list will be simpler to get it working.
 */
struct StockEntry {
	char* stock;
	float price;

	struct StockEntry* next;
};

/*
 * Basically a hashtable geared towards storing stock info
 *
 * Namely instead of using a generic char* to store binary data a float is used
 * since you'd never want to store anything else other than stock prices.
 * Saves some time since you can skip using memcpy to insert data.
 * Also no need for the user to use pointers or anoymous arrays
 */
struct StockTable {
	struct StockEntry* entries;
	size_t size; // Larger size means less chance of collisons, so make this baby large!!!
};

bool stockTableNew(struct StockTable* table, size_t size);
bool stockTableSet(char* key, float value);
bool stockTableGet(char* key, float* out);

uint32_t stockTableHash(char* key, size_t keyLen, size_t tableSize); // generates index for a given stock name. Not sure what algo to use


#endif
