/* The project is broken up into four files:
 * main.c - Basic app logic
 * conf.c/.h - Parses .conf file for stock prices and budget info
 * stocktable.c/.h - Hashtable implementation that stores StockEntry structs, which have stock info in them (price, shares owned, etc)
 * command.c/.h - Parses and executes command strings
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "stocktable.h"
#include "conf.h"

void printCollisons(int arr[], int count);

int main(int argc, char* argv[]){

	struct StockTable table;
	stockTableNew(&table, 10000);

	printf("Size: %d\n", table.size);

	float budget = 0, threshold = 0;
	parseConf("prices.conf", &budget, &threshold, &table);	

	printf("Budget: %f, Threshold: %f\n\n", budget, threshold);

	struct StockEntry* entry = stockTableGetEntry(&table, "AMD");

	if (entry == NULL) {
		printf("Could not find stock entry you fuck\n");
	} else {
		printf("%s, %f\n", entry->stock, entry->price);
	}

	return EXIT_SUCCESS;
}

void printCollisons(int arr[], int count){

	for (int i = 0; i < count - 1; i++){
		for (int j = i + 1; j < count; j++){
			if (arr[i] == arr[j]){
				printf("COLLISION: %d\n", arr[i]);
			}
		}	
	}
}	
