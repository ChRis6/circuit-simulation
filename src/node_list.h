#ifndef NODE_LIST_H
#define NODE_LIST_H

#include "nodes.h"
#include "circuit_hash.h"

#define METHOD_LU       1
#define METHOD_CHOLESKY 2

#define PLOT_ON  1
#define PLOT_OFF 0

/* a single node  list*/
typedef struct list_node{

	char type;
	NODE node;

	struct list_node* next;
}LIST_NODE;

typedef struct dc_sweep{

	char *name;

	LIST_NODE* node;
	double start_v;
	double end_v;
	double inc;

	double oldval;
	
}DC_SWEEP_T;


/* List */
typedef struct list{
	int len;

	LIST_NODE* head;

	hashtable_t *hashtable;
	char has_reference;
	int m2;

	char solving_method;
	DC_SWEEP_T dc_sweep;
	char plot;

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

/*
 * search the list for the name specified
 *	Returns: 1 when found
 *			 0 otherwise
 */
LIST_NODE* list_search_by_name( LIST* list , char *name);
#endif