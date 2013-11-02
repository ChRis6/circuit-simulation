#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "circuit_hash.h"

/*
 * Init hash table
 *
 */
hashtable_t* ht_init( int size ){

	hashtable_t* hashtable;
	int i;

	if( size < 1)
		return NULL;

	hashtable = ( hashtable_t* ) malloc( sizeof(hashtable_t));
	if( !hashtable )
		return NULL;

	hashtable->table = ( entry_t**) malloc( sizeof(entry_t*) * size );
	if( !(hashtable->table) ){
		free(hashtable);
		return NULL;
	}

	for( i = 0; i < size ; i++ )
		hashtable->table[i] = NULL;
	hashtable->size = size;

	return hashtable;

}

/*
 * One-at-a-Time hash
 * Bob Jenkins
 */
unsigned int hash(hashtable_t *hashtable,char* key){

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
	return h % hashtable->size ;
}

/*
 * Creates a new key - value pair ( circuit node name - int )
 */
entry_t* ht_create_pair(char* key,int value){
	
	entry_t* newpair;

	if( !key )
		return NULL;

	newpair = ( entry_t* ) malloc( sizeof(entry_t));
	if( !newpair)
		return NULL;
	
	newpair->key = strdup(key);
	if( !(newpair->key)){
		free( newpair );
		return NULL;
	}
	newpair->value = value;
	newpair->next = NULL;

	return newpair;
}

/*
 * add pair
 *
 * return:  > 0 success
 *		   == 0 no memory
 *          < 0 key already exists( no value replacement )
 */
int ht_insert_pair(hashtable_t *hashtable, char* key, int value ){

	entry_t* newpair = NULL;
	entry_t* curr = NULL;
	entry_t* head = NULL;
	int index;

	if( !key || !hashtable )
		return 0;

	/* first make a new pair and then try to insert */
	newpair = ht_create_pair(key , value);
	if( !newpair )
		return 0;

	index = hash(hashtable , key );
	head = hashtable->table[index];

	if( head == NULL ){
		/* no element is here.just insert */
		
		hashtable->table[index] = newpair;
		newpair->next = NULL;

		/* success */
		return 1;
	}
	else{

		/* there are elements here */
		for( curr = hashtable->table[index] ; curr ; curr = curr->next){
			if( strcmp(curr->key , key ) == 0 )
				/* key already exists.DONT REPLACE */
				free(newpair->key);
				free(newpair);
				//printf("key : \"%s\" already exits at index: %d\n",key,index);
				return -1;
		}
		/* no matching key found on the list.*/
		newpair->next = hashtable->table[index];
		hashtable->table[index] = newpair;

		/* success */
		return 1;
	}

	return 0;
}

/*
 * get value
 *
 * Returns: 1 = success. ret points to value of the matching key
 *			0 = key not found
 */
int ht_get(hashtable_t* hashtable , char* key , int *ret){
	
	int index;
	entry_t* curr = NULL;

	if( !hashtable || !key || !ret )
		return 0;

	index = hash( hashtable , key );
	for( curr = hashtable->table[index] ; curr; curr = curr->next ){
		if( strcmp( key , curr->key ) == 0 ){
			*ret = curr->value;
			return 1;
		}
	}

	/* key not found */
	return 0;
}


/*
 * print all pairs
 */
void ht_print(hashtable_t* hashtable){

	int i;
	entry_t* curr;

	if( !hashtable )
		return;

	printf("------ PRINTING HASH TABLE -----------\n");
	for( i = 0 ; i < hashtable->size ; i++){
		if( hashtable->table[i] != NULL ){
			printf("Hash Table index: %d \n",i);

			for( curr = hashtable->table[i] ; curr ; curr = curr->next){
				printf("Pair : key = \"%s\" value = %d\n",curr->key,curr->value);
			}

		}
	}

}


/*
 * Deallocate
 */
void ht_free(hashtable_t *hashtable){

	entry_t* dummy;
	int i;
	
	if( !hashtable )
		return;

	for( i = 0 ; i < hashtable->size ; i++){

		while( hashtable->table[i] ){
			dummy = hashtable->table[i]->next;
			free(hashtable->table[i]->key);
			free(hashtable->table[i]);
			hashtable->table[i] = dummy;
		}
	}

	free( hashtable->table );
	free(hashtable);
}