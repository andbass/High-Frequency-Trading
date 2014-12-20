#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stocktable.h"

int main(int argc, char* argv[]){

	struct StockTable table;
	stockTableNew(&table, 100);

	stockTableSet(&table, "TEST", 4);

	

	return EXIT_SUCCESS;
}
