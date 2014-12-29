
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

	float budget = 0, threshold = 0;
	parseConf("prices.conf", &budget, &threshold, &table);	

	struct StockEntry* entry = stockTableGetEntry(&table, "AOL");

	printf("%s, %f\n", entry->stock, entry->price);

	clock_t start = clock();
	stockTableFree(&table);
	clock_t end = clock();

	printf("%ld\n", end - start);

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
