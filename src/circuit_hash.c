#include "circuit_hash.h"
#include <string.h>

/*
 * One-at-a-Time hash
 * Bob Jenkins
 *
unsigned int hash(char* key){


	unsigned h = 0;
    int i;
    int len;

    len = strlen(key);
  
	for ( i = 0; i < len; i++ ) {
    	h += key[i];
    	h += ( h << 10 );
    	h ^= ( h >> 6 );
	}
 
	h += ( h << 3 );
	h ^= ( h >> 11 );
	h += ( h << 15 );
	return h;
}
*/