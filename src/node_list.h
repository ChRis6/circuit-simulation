#ifndef NODE_LIST_H
#define NODE_LIST_H

#include "nodes.h"
#include "circuit_hash.h"

#define METHOD_LU       1
#define METHOD_CHOLESKY 2
#define METHOD_CG       3
#define METHOD_BICG     4


#define METHOD_LU_SPARSE       5
#define METHOD_CHOLESKY_SPARSE 6
#define METHOD_CG_SPARSE       7
#define METHOD_BICG_SPARSE     8

#define METHOD_TR 9 //Transient Trapezoidal
#define METHOD_BE 10 //Backward-Euler


#define PLOT_ON  1
#define PLOT_OFF 0

extern char *solving_method_names[20];

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

	double itol;
	unsigned int nz;
	char sparse;
	char transient_sim;

	double time_step;
	double fin_time;
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

/**
 * Creates an empty pair list
 */
PAIR_LIST* create_pair_list();

/**
 * Adds a pair to the list
 */
int add_to_pair_list(PAIR_LIST* pair_list, double ti, double ii);

#endif