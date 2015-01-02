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

FILE* getInputFile(int argc, char* argv[]){

	FILE* inputFile = NULL;

	if (argc == 1) {
		inputFile = stdin;
	} else if (argc == 2){
		inputFile = fopen(argv[1], "r");

		if (inputFile == NULL){
			printf("Error: Invalid input file specificed\n");
			exit(EXIT_FAILURE);
		}
	} else {
		printf("Usage: hft [input file]\n");
		exit(EXIT_FAILURE);
	}
	return inputFile;
}

/*
 * Prepares stocktable, parses prices.conf, and handles input / program logic.
 */
int main(int argc, char* argv[]){

	// Inital file IO
	FILE* inputFile = getInputFile(argc, argv);
	FILE* outputFile = fopen("output.txt", "w");
	FILE* executedFile = fopen("executed.txt", "w");

	if (outputFile == NULL || executedFile == NULL) {
		printf("Error: could not create necessary output files\n");
	}

	struct StockTable table;
	stockTableNew(&table, 10000);

	double budget = 0, threshold = 0;
	parseConf("prices.conf", &budget, &threshold, &table);	
	
	return EXIT_SUCCESS;
}	
