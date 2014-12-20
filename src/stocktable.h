#ifndef MARKET_TABLE_H
#define MARKET_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* A linked list meant to be stored within a StockTable. */
struct StockEntry {
	char* stock;
	float price;

	struct StockEntry* next;
};

/*
 * Basically a hashtable geared towards storing stock info.
 */
struct StockTable {
	struct StockEntry* entries;
	size_t size; // Larger size means less chance of collisons, so make this baby large!!!
	size_t bitMask; // this is ANDed with hash.
};

bool stockTableNew(struct StockTable* table, size_t approxSize);
bool stockTableSet(struct StockTable* table, char* key, float value);
bool stockTableGet(struct StockTable* table, char* key, float* out);

uint32_t stockTableHash(char* key, size_t keyLen, size_t tableSize); // generates index for a given stock name. Not sure what algo to use
int highestOrderBit(int n);

#endif
