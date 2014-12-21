
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stocktable.h"

void printCollisons(int arr[], int count);

int main(int argc, char* argv[]){
	struct StockTable table;
	stockTableNew(&table, 10000);

	stockTableSet(&table, "Andrew", 420);

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
