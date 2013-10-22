#ifndef NODE_LIST_H
#define NODE_LIST_H

#include "nodes.h"

/* a single node a list*/
typedef struct list_node{

	int type;
	NODE node;

	struct list_node* next;
}LIST_NODE;

/* List */
typedef struct list{
	int len;
	LIST_NODE* head;
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
 * dealocate a list
 */
void free_list(LIST list);
#endif