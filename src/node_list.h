#ifndef NODE_LIST_H
#define NODE_LIST_H

#include "nodes.h"
#include "circuit_hash.h"

/* a single node  list*/
typedef struct list_node{

	char type;
	NODE node;

	struct list_node* next;
}LIST_NODE;

/* List */
typedef struct list{
	int len;
	LIST_NODE* head;

	hashtable_t *hashtable;
	char has_reference;
}LIST;




/****************************** PROTOTYPES ******************************/

/*
 *Initiliaze a list
 */
void init_list(LIST* list);

/*
 * Prints list
 */
void print_list(LIST list);

/*
 * deallocate a list
 */
void free_list(LIST* list);

/*
 *
 * add circuit element to list
 */
int add_to_list(LIST* list,int type,void* element,int size);

/*
 * add node element to list 
 * 
 * returns: 1 success
 *          0 fail
 */
int add_node_to_list( LIST* list, NODE* circuit_node , int type);
#endif