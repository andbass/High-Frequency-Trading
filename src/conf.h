#ifndef CONF_H
#define CONF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stocktable.h"

/*
 * Parses a .conf file for the budget and threshold.
 * Populates a StockTable with the specified stock prices.
 * 
 * If an error is encountered during parsing, the function will
 * kill the program and display an error message.
 */
void parseConf(char* path, double* budget, double* threshold, struct StockTable* table);

#endif
