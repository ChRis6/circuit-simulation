#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node_list.h"

#define HASH_SIZE 10000

/*
 *Initiliaze a list
 */
void init_list(LIST* list){
	list->len = 0;
	list->head = NULL;

	list->hashtable = ht_init( HASH_SIZE );
	if( !(list->hashtable)){
		printf("No memory for hash table...\n");
		exit(1);
	}
	list->has_reference = 0;
}

/*
 * Prints list
 */
void print_list(LIST list){

	int i;

	printf("------------ PRINTING NODE LIST------------------\n\n");
	
	for( i = 0 ; i < list.len ; i++ ){
		if( list.head->type == NODE_RESISTANCE_TYPE ){
			printf("RESISTANCE : %s <%d> <%d> %f\n",list.head->node.resistance.name , list.head->node.resistance.node1 , list.head->node.resistance.node2 , list.head->node.resistance.value );
		}
		else if( list.head->type == NODE_CAPACITY_TYPE ){
			printf("CAPACITY : %s <%d> <%d> %e \n",list.head->node.capacity.name , list.head->node.capacity.node1 , list.head->node.capacity.node2 , list.head->node.capacity.value );
		}
		else if( list.head->type == NODE_INDUCTANCE_TYPE ){
			printf("INDUCTANCE : %s <%d> <%d> %f\n",list.head->node.inductance.name , list.head->node.inductance.node1 , list.head->node.inductance.node2 , list.head->node.inductance.value );
		}
		else if( list.head->type == NODE_SOURCE_V_TYPE ){
			printf("VOLTAGE SOURCE: %s <%d> <%d> %f\n",list.head->node.source_v.name , list.head->node.source_v.node1 , list.head->node.source_v.node2 , list.head->node.source_v.value );
		}
		else if( list.head->type == NODE_SOURCE_I_TYPE ){
			printf("CURRENT SOURCE: %s <%d> <%d> %f\n",list.head->node.source_i.name , list.head->node.source_i.node1 , list.head->node.source_i.node2 , list.head->node.source_i.value );
		}
		else if( list.head->type == NODE_MOSFET_TYPE ){
			printf("MOSFET TRANSISTOR: %s <%d> <%d> <%d> <%d> %e %e \n",list.head->node.mosfet.name , list.head->node.mosfet.drain , list.head->node.mosfet.gate, list.head->node.mosfet.source , list.head->node.mosfet.body , list.head->node.mosfet.l , list.head->node.mosfet.w );
		}
		else if( list.head->type == NODE_BJT_TYPE ){
			printf("BJT TRANSISTOR: %s <%d> <%d> <%d> \n",list.head->node.bjt.name , list.head->node.bjt.collector , list.head->node.bjt.base  , list.head->node.bjt.emitter );
		}

		list.head = list.head->next;
	}
}

/*
 * deallocate a list
 */
void free_list(LIST* list){

	LIST_NODE* dummy;
	while( list->head){
		dummy = list->head->next;
		free(list->head);
		list->head = dummy;
	}

	list->len = 0; 
	ht_free(list->hashtable);
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
	if( type == NODE_RESISTANCE_TYPE){
		memcpy( &(new->node.resistance) , element , size );
	}
	else if( type == NODE_CAPACITY_TYPE){
		
		memcpy( &(new->node.capacity) , element , size );
	}
	else if( type == NODE_INDUCTANCE_TYPE){

		memcpy( &(new->node.inductance) , element , size );
	}
	else if( type == NODE_SOURCE_V_TYPE ){
		memcpy( &(new->node.source_v) , element , size );
	}
	else if( type == NODE_SOURCE_I_TYPE ){
		memcpy( &(new->node.source_i) , element , size );
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
		//new->next = list->head->next;
		new->next = list->head;
		//list->head->next = new;
		list->head = new;
	}

	/* success */
	return 1;

}


/*
 * Simply add circuit node to list
 */
int add_node_to_list( LIST* list, NODE* circuit_node , int type){

	if( !list || !circuit_node)
		return 0 ;

	LIST_NODE* new = NULL;
	new = malloc( sizeof(LIST_NODE));
	if( !new )
		return 0;

	/* fill new  LIST_NODE */
	new->type = type;
	memcpy( &(new->node) , circuit_node , sizeof(NODE) );

	/* link to list */
	if( list->len > 0 ){
		list->len = list->len + 1;
		new->next = list->head;
		list->head = new;

	}
	else{

		list->len = 1;
		new->next = NULL;
		list->head = new;
	}

	return 1;
}