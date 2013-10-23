#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node_list.h"

/*
 *Initiliaze a list
 */
void init_list(LIST* list){
	list->len = 0;
	list->head = NULL;
}

/*
 * Prints list
 */
void print_list(LIST list){

	int i;

	printf("------------ PRINTING NODE LIST------------------\n\n");
	
	for( i = 0 ; i < list.len ; i++){
		if( list.head->type == NODE_RCL_TYPE ){
			printf("RESISTANCE: %s <%s> <%s> %f\n",list.head->node.rcl.name , list.head->node.rcl.node1 , list.head->node.rcl.node2 , list.head->node.rcl.value );
		}
		else if( list.head->type == NODE_SOURCE_V_TYPE ){
			printf("VOLTAGE SOURCE: %s <%s> <%s> %f\n",list.head->node.rcl.name , list.head->node.rcl.node1 , list.head->node.rcl.node2 , list.head->node.rcl.value );
		}
		list.head = list.head->next;
	}
}

/*
 * dealocate a list
 */
void free_list(LIST list){

}
/*
 *
 * add element to list
 */
int add_to_list( LIST* list, int type, void* element, int size ){

	if( size <= 0)
		return 0;

	/* allocate memory for the new node */
	LIST_NODE* new = (LIST_NODE*) malloc(sizeof(LIST_NODE));
	if(!new)
		return 0;

	new->type = type;
	if( type == NODE_RCL_TYPE){

		memcpy( &(new->node.rcl) , element , size );
	}
	else if( type == NODE_SOURCE_V_TYPE ){
		memcpy( &(new->node.source_v) , element , size );
	}
	else if( type == NODE_SOURCE_I_TYPE ){
		//memcpy( &(new->node.rcl) , element , size );

	}
	else if( type == NODE_MOSFET_TYPE ){
		memcpy( &(new->node.mosfet) , element , size );

	}
	else if( type == NODE_BJT_TYPE ){
		memcpy( &(new->node.bjt) , element , size );
	}
	else{	// no matching type
		free(new);
		return 0;
	}

	/* link new node the list */
	if( list->len == 0 ){
		/* the list is empty */
		list->len = 1 ;

		list->head = new;
		new->next = NULL;
		
	}
	else{
		/* non empty list */
		list->len = list->len + 1 ;
		new->next = list->head->next;
		list->head->next = new;
	}

	/* success */
	return 1;

}