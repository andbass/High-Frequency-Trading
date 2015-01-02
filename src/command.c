#include "command.h"
#include <string.h>

// Seperator used in command string
const char* SEPERATORS = " ";

inline void freeCommand(struct Command* cmd){
	free(cmd->stock);
}

bool parseCommand(char* str, struct Command* cmd){

	// Need to create new string, as strtok modifies its first argument
	char copy[strlen(str) + 1];
	strcpy(copy, str);

	// Get action, B or S
	char* tok = strtok(copy, SEPERATORS);

	// Should only be one character
	if (strlen(tok) != 1) return false;

	if (strcmp(tok, "B") == 0){
		cmd->action = BUY;
	} else if (strcmp(tok, "S") == 0){
		cmd->action = SELL;
 	} else {
		return false;
	}
	
	// Now, lets get the stock
	tok = strtok(NULL, SEPERATORS);

	cmd->stock = malloc(sizeof(char) * (strlen(tok) + 1));
	strcpy(cmd->stock, tok);

	// Great, time for the number of shares
	tok = strtok(NULL, SEPERATORS);

	char* endPtr = NULL;
	cmd->shares = strtol(tok, &endPtr, 10);

	if (tok == endPtr) return false; // if endPtr points back to string storing number, that means strtol failed

	// Oh boy, one more! The safety
	tok = strtok(NULL, SEPERATORS);

	// Like the action, should only be one char	
	if (strlen(tok) != 1) return false;

	if (strcmp(tok, "S") == 0) {
		cmd->safe = true;
	} else if (strcmp(tok, "U") == 0) {
		cmd->safe = false;
	} else {
		return false;
	}

	return true;
}

bool execCommand(struct Command* cmd, struct StockTable* table, float* budget, const float threshold){
	struct StockEntry* entry = stockTableGetEntry(table, cmd->stock);	
	if (entry == NULL) return false;

	float newBudget;
	switch (cmd->action){
		
		case BUY:
			newBudget = *budget - cmd->shares * entry->price;

			if (cmd->safe && newBudget < threshold){
				printf("Error: Budget amount (%f) would fall below threshold (%f) if %d shares of %s were to be bought", *budget, threshold, cmd->shares, entry->stock);	
				return false;			
			} else if (newBudget < 0){
				printf("Error: Budget amount (%f) would be negative if %d shares of %f were to be sold", *budget, cmd->shares, entry->stock);	
				return false;
			} 	
			entry->sharesOwned += cmd->shares;
			*budget = newBudget;

			break;

		case SELL:
			if (cmd->shares > entry->sharesOwned) {
				printf("Error: could not sell %d shares of %s, as you only own %d\n", cmd->shares, entry->stock, entry->sharesOwned);
				return false;
			}
			entry->sharesOwned -= cmd->shares;
			*budget += entry->price * cmd->shares;
			break;
	}

	return true;
}
