#include "command.h"

bool execCommand(struct Command* cmd, struct StockTable* table, float* budget, const float threshold){
	struct StockEntry* entry = stockTableGetEntry(table, cmd->stock);	
	if (entry == NULL) return false;

	switch (cmd->action){
		
		case BUY:
			
			break;

		case SELL:
			
			break;
	}

	return NULL;
}
