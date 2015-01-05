#include "command.h"
#include <string.h>

// Seperator used in command string
const char* SEPERATORS = " \n";

/*
 * Extracts relevant info out of a string, stores it into a Command
 */
bool parseCommand(char* str, struct Command* cmd){
	// Need to create new string, as strtok modifies its first argument
	char copy[strlen(str) + 1];
	strcpy(copy, str);
		
	// Get action, B or S
	char* tok = strtok(copy, SEPERATORS);

	// Should only be one character
	if (strlen(tok) != 1) {
		fprintf(stderr, "Error: Action should only be one character long\n");
		return false;
	}	

	if (strcmp(tok, "B") == 0){
		cmd->action = BUY;
	} else if (strcmp(tok, "S") == 0){
		cmd->action = SELL;
 	} else {
		fprintf(stderr, "Error: %s is an invalid action\n", tok);
		return false;
	}
	
	// Now, lets get the stock
	tok = strtok(NULL, SEPERATORS);
	
	if (strlen(tok) + 1 > MAX_KEY_LENGTH){
		fprintf(stderr, "Error: stock name is greater than %d characters\n", MAX_KEY_LENGTH);
		return false;
	}
	strcpy(cmd->stock, tok);

	// Great, time for the number of shares
	tok = strtok(NULL, SEPERATORS);

	char* endPtr = NULL;
	cmd->shares = strtol(tok, &endPtr, 10);

	if (tok == endPtr) {
		fprintf(stderr, "Error: could not parse %s into a number\n", tok);
		return false; // if endPtr points back to string storing number, that means strtol failed
	}

	// Oh boy, one more! The safety
	tok = strtok(NULL, SEPERATORS);

	// Like the action, should only be one char	
	if (strlen(tok) != 1) {
		fprintf(stderr, "Error: The safety indicator should only be one character: %s\n", tok);
		return false;
	}	

	if (strcmp(tok, "S") == 0) {
		cmd->safe = true;
	} else if (strcmp(tok, "U") == 0) {
		cmd->safe = false;
	} else {
		fprintf(stderr, "Error: %s is an invalid safety symbol\n", tok);
		return false;
	}

	return true;
}

bool execCommand(struct Command* cmd, struct StockTable* table, double* budget, const double threshold){
	struct StockEntry* entry = stockTableGetEntry(table, cmd->stock);	
	if (entry == NULL) {
		fprintf(stderr, "Error: %s is not a stock that was specified in the .conf file (remember, stock names are case-senstive)\n", cmd->stock);	
		return false;
	}

	double newBudget;
	switch (cmd->action){
		
		case BUY:
			newBudget = *budget - cmd->shares * entry->price;
			
			if (cmd->safe && newBudget < threshold){
				fprintf(stderr, "Error: budget amount (%f) would fall below threshold (%f) if %d shares of %s were to be bought\n", *budget, threshold, cmd->shares, entry->stock);	
				return false;			
			} else if (newBudget < 0){
				fprintf(stderr, "Error: budget amount (%f) would be negative if %d shares of %s were to be bought\n", *budget, cmd->shares, entry->stock);	
				return false;
			}

			entry->sharesOwned += cmd->shares;
			*budget = newBudget;

			break;

		case SELL:
			if (cmd->shares > entry->sharesOwned) {
				fprintf(stderr, "Error: could not sell %d shares of %s, as you only own %d\n", cmd->shares, entry->stock, entry->sharesOwned);
				return false;
			}

			entry->sharesOwned -= cmd->shares;
			*budget += entry->price * cmd->shares;

			break;
	}

	return true;
}
