#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stocktable.h"

/*
 * Whether to buy or sell shares of a stock
 */
enum Action {
	BUY,
	SELL,
};

/*
 * Represents user input. For example:
 * B AAPL 200 S
 * B -> Buy stock
 * AAPL -> The stock to buy
 * 200 -> How many shares
 * S -> Whether to allow the budget to fall below the threshold, if buying
 */
struct Command {
	enum Action action;
	char stock[MAX_KEY_LENGTH]; 
	int shares;
	bool safe;
};

/*
 * Parses a command in string form and converts it into a Command struct.
 */
bool parseCommand(char* str, struct Command* cmd);

/*
 * Executes command insturctions, as encoded within a Command stuct.
 * Returns whether the command was possible to execute.
 */
bool execCommand(struct Command* cmd, struct StockTable* table, double* budget, const double threshold);

#endif
