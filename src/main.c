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
#include "command.h"

void printCollisons(int arr[], int count);
void printStock(struct StockEntry* entry);

int main(int argc, char* argv[]){

	struct StockTable table;
	stockTableNew(&table, 10000);

	float budget = 0, threshold = 0;
	parseConf("prices.conf", &budget, &threshold, &table);	

	char* cmdStr = "S AAPL 2 S";

	struct Command cmd;
	parseCommand(cmdStr, &cmd);
	printf("Action: %s, Stock: %s, Shares: %d, Safety: %d\n", (cmd.action == BUY) ? "BUY" : "SELL", cmd.stock, cmd.shares, cmd.safe);
	printf("\n");

	struct StockEntry* entry = stockTableGetEntry(&table, "AAPL");
	entry->sharesOwned = 10;

	printf("Before: ");
	printStock(entry);
	printf("Budget: %f\n\n", budget);

	execCommand(&cmd, &table, &budget, threshold);

	printf("After: ");
	printStock(entry);
	printf("Budget: %f\n", budget);

	return EXIT_SUCCESS;
}

void printStock(struct StockEntry* entry){
	printf("Stock: %s, Owned: %d, Price: %f\n", entry->stock, entry->sharesOwned, entry->price);
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
