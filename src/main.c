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

#define LINE "===================================\n"

/*
 * Will get either the input file specified or return stdin, depending on the commandline arguments.
 */
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
 * Gets user input and prints a '>' to the screen.
 * Requires a FILE* parameter to match function pointer
 */
char* getInputFromUser(char* buf, int size, FILE* file){
	printf("> ");
	return fgets(buf, size, file);
}

/*
 * Prepares stocktable, parses prices.conf, and handles input / program logic.
 */
int main(int argc, char* argv[]){

	// Inital file IO
	FILE* inputFile = getInputFile(argc, argv);
	FILE* outputFile = freopen("output.txt", "w", stdout);
	FILE* executedFile = fopen("executed.txt", "w");

	if (outputFile == NULL || executedFile == NULL) {
		printf("Error: could not create necessary output files\n");
	}

	// Setting up hash table and parsing conf
	struct StockTable table;
	stockTableNew(&table, 10000);

	double budget = 0, threshold = 0;
	parseConf("prices.conf", &budget, &threshold, &table);	

	// Entering program loop
	char buffer[4096];
	struct Command cmd;

	char* (*getInput)(char*, int, FILE*);
	if (inputFile == stdin) {
		getInput = &getInputFromUser;
	} else {
		getInput = &fgets;
	}
	
	char* initialMessage = "";
	char* closingMessage = "";
	if (inputFile == stdin) {
		initialMessage = "Press Ctrl + D to exit\n";
		closingMessage = "\n";
	}

	puts(initialMessage);
	while (true) {
		char* line = getInput(buffer, 4096, inputFile);
		
		if (line == NULL) {
			break;
		}

		if (parseCommand(line, &cmd)){
			if (execCommand(&cmd, &table, &budget, threshold)){
				
				// Output executed command string as is
				fputs(line, executedFile);	
			}	
		}
	}
	
	fclose(inputFile);
	fclose(executedFile);
	fclose(outputFile);

	puts(closingMessage);
	return EXIT_SUCCESS;
}	
