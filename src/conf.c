
#include "conf.h"
#include <string.h>
#include <ctype.h>

/*
 * Extracts out a float value that is after an equals sign '='
 * Used for budget and threshold extraction
 * Can specify temporary buffer used
 */
inline static float getFloat(FILE* file, char buf[]){
	int ch = getc(file);
	while (ch != EOF && ch != '='){
		ch = getc(file);
	} 

	fgets(buf, 1024, file); // should be budget line

	char* endPtr;
	float val = strtof(buf, &endPtr);

	if (endPtr == buf) {
		printf("Error: could not parse float value\n");
		exit(EXIT_FAILURE);
	}
	return val;
}

/*
 * Parses a .conf file for the budget, threshold, and stock prices.  
 */
void parseConf(char* path, float* budget, float* threshold, struct StockTable* table){
	FILE* file = fopen(path, "r");

	// budget and threshold
	char buf[1024];
	
	*budget = getFloat(file, buf);
	*threshold = getFloat(file, buf);
	
	
}
