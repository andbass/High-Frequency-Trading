#include "conf.h"

#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "stocktable.h"

static const int FAILURE = 1;
static const int SUCCESS = 0;

/*
 * Extracts out a double value that is after an equals sign '='
 * Used for budget and threshold extraction
 * Can specify temporary buffer used
 */
inline static double getFloat(FILE* file, char buf[], size_t size){
	int ch = 0;
	do {
		ch = getc(file);
	} while (ch != EOF && ch != '=');

	fgets(buf, size, file); // everything after equals sign should be the double value

	char* endPtr;
	double val = strtod(buf, &endPtr);

	if (endPtr == buf) {
		printf("Error: could not parse double value\n");
		exit(EXIT_FAILURE);
	}
	return val;
}

inline static void gotoNextLine(FILE* file){
	int ch = 0;
	do {
		ch = getc(file);
	} while (ch != EOF && ch != '\n');
}

/*
 * Extracts a key-value pair out of conf file.  For instance: TEST=10
 * Supports comments, returns whether the file has reached EOF or not.
 *
 * Returns EOF if end of file is reached, 0 if parsed successfully,
 * or 1 if an error is encountered in parsing.
 */
inline static int insertKeyValuePair(FILE* file, struct StockTable* table, char buf[], size_t size){
	unsigned int i = 0;
	int ch = 0;
	while (i < size - 1){ // why (size - 1)?  leave room for null terminator
		ch = getc(file);
	
		// by skipping whitespace, we avoid having to later trim the string at the cost of disallowing spaces in key names.  
		// an ok trade off here, since stock tickets never have spaces anyways
		if (!isspace(ch)) {
			if (ch == '=') {
				break;
			} else if (ch == '#') { 
				gotoNextLine(file);
				return SUCCESS;
			} else if (ch == EOF) {
				return EOF;
			}
			
			buf[i] = ch;
			++i;
		}

	}
	buf[i] = '\0';

	char numberBuf[2048];
	if (fgets(numberBuf, 2048, file) == NULL) {
		return EOF;
	}

	char* endPtr = NULL;
	double val = strtod(numberBuf, &endPtr);

	// this is how strtof lets you know if parsing failed, if the endPtr address is equal to the source pointer string
	if (endPtr == numberBuf) {
		return FAILURE;
	}
	
	if (!stockTableSetPrice(table, buf, val)){
		printf("Could not insert %s (%f) into table\n", buf, val);
	}

	return SUCCESS;
}

/*
 * Parses a .conf file for the budget, threshold, and stock prices.  
 */
void parseConf(char* path, double* budget, double* threshold, struct StockTable* table){
	FILE* file = fopen(path, "r");
	
	if (file == NULL) {
		printf("Invalid file path\n");
		exit(EXIT_FAILURE);
	}

	// budget and threshold
	char buf[1024];
	
	*budget = getFloat(file, buf, 1024);
	*threshold = getFloat(file, buf, 1024);
	
	int returnCode = 0;
	while ((returnCode = insertKeyValuePair(file, table, buf, 1024)) != EOF){
		if (returnCode == FAILURE){
			printf("Error encountered in parsing key value pairs in conf file\n");
			exit(EXIT_FAILURE);
		}	

	}

	fclose(file);
}
