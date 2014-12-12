#include "stocktable.h"

#define FNV_OFFSET_BASIS 2166136261
#define FNV_PRIME 16777619

/*
 * The following hash function uses FNV-1a.
 *
 * I chose it because it seemed to be very fast
 * and avoided collisons well in a table with 
 * 100 elements.
 * Its also simple.
 *
 * Compared to FNV-1, the website www.isthe.com reported that FNV-1a had better
 * dispertion for data with less than 4 octets of data.  Since the stocks would only be
 * 3-4 octets of data (assuming a char is a byte, or 8 bits), FNV-1a seems to be a better choice.
 * I have not tested this though.
 *
 * The algo is public domain:
 * http://www.isthe.com/chongo/tech/comp/fnv/
 */

uint32_t stockTableHash(char* key, size_t strLen, size_t tableSize){
	uint32_t hash = FNV_OFFSET_BASIS;
	for (int i = 0; i < strLen; i++){ // magic!
		hash ^= *(key + i); // get char in index i in string
		hash *= FNV_PRIME;  
	}
	return hash % tableSize;
}
