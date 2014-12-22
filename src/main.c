
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stocktable.h"
#include "conf.h"

void printCollisons(int arr[], int count);

int main(int argc, char* argv[]){
	struct StockTable table;
	stockTableNew(&table, 10000);

	float budget = 0, threshold = 0;

	parseConf("prices.conf", &budget, &threshold, &table); 

	printf("Budget: %f\n", budget);
	printf("Threshold: %f\n", threshold);

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
