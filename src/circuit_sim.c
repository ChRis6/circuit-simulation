#include "circuit_sim.h"


/*
 * Create the matrix and vector for the circuit elements
 */
int create_mna(LIST *list , gsl_matrix **matrix , gsl_vector** vector){

	LIST_NODE* curr;
	gsl_matrix* tmp_matrix;
	gsl_vector* tmp_vector;

	//int group1 = list->len - list->m2;
	//int group2 = list->m2;
	int m2_elements_found = 0;

	int rows;
	int columns;

	if( !matrix || !vector || !list)
		return 0;


	/* allocate matrix and vector */
	rows    = list->hashtable->num_nodes + list->m2;
 	columns = list->hashtable->num_nodes + list->m2;

 	printf("Creating matrix: rows = %d columns = %d\n",rows,columns);

 	tmp_matrix = gsl_matrix_calloc(rows , columns);
 	if( !tmp_matrix )
 		return 0;

 	tmp_vector = gsl_vector_calloc( rows);
 	if( !tmp_vector )
 		return 0;	

	/* compute mna */
 	for( curr = list->head ; curr ; curr = curr->next){

 		/*
 		 * RESISTANCE ELEMENT
 		 */

 		if( curr->type == NODE_RESISTANCE_TYPE ){

 			double conductance = 1 / curr->node.resistance.value ;
 			int plus_node  = curr->node.resistance.node1 - 1 ;
 			int minus_node = curr->node.resistance.node2  - 1;

 			/* <+> is ground */
 			if( plus_node == -1 ){

 				double value = gsl_matrix_get(tmp_matrix , minus_node , minus_node);
 				value += conductance ; 
 				gsl_matrix_set( tmp_matrix , minus_node , minus_node ,  value );
 				//printf("Adding to matrix element (%d,%d) value:%f\n\n",minus_node,minus_node,value);
 			}
 			else if( minus_node == -1  ){
 				/* <-> is ground */
 				double value = gsl_matrix_get(tmp_matrix , plus_node , plus_node);
 				value += conductance; 
 				gsl_matrix_set( tmp_matrix , plus_node ,plus_node , value );
				//printf("Adding to matrix element (%d,%d) value:%f\n\n",plus_node,plus_node,value);
 			}
 			else {

 				/* set <+> <+> matrix element */
 				double value;
 				

 				value = gsl_matrix_get(tmp_matrix , plus_node , plus_node);
 				value += conductance ; 
 				gsl_matrix_set(tmp_matrix , plus_node , plus_node , value);
				//printf("Adding to matrix element (%d,%d) value:%f\n",plus_node,plus_node,value);

 				/* set <+> <-> */
 				value = gsl_matrix_get(tmp_matrix , plus_node , minus_node);
 				value -= conductance ; 
 				gsl_matrix_set(tmp_matrix , plus_node , minus_node , value);
				//printf("Adding to matrix element (%d,%d) value:%f\n",plus_node,minus_node,value);

 				/* set <-> <+> */
 				value = gsl_matrix_get(tmp_matrix , minus_node , plus_node);
 				value -= conductance ; 
 				gsl_matrix_set(tmp_matrix , minus_node , plus_node , value); 				
				//printf("Adding to matrix element (%d,%d) value:%f\n",minus_node,plus_node,value);

 				/* set <-> <-> */
 				value = gsl_matrix_get(tmp_matrix , minus_node , minus_node);
 				value += conductance ; 
 				gsl_matrix_set(tmp_matrix , minus_node , minus_node , value);
				//printf("Adding to matrix element (%d,%d) value:%f\n\n",minus_node,minus_node,value);
 			}
 		}
 		/* 
 		 * CURRENT SOURCE
 		 */
 		else if( curr->type == NODE_SOURCE_I_TYPE ){

 			/* change only the vector */
 			double current = curr->node.source_i.value;
 			double value;

 			if( curr->node.source_i.node1 != 0 ){
 				/* ste <+> */
 				value  = gsl_vector_get(tmp_vector , curr->node.source_i.node1 - 1  );
 				value -= current;
 				gsl_vector_set(tmp_vector , curr->node.source_i.node1 -1  , value );
 			}

 			if( curr->node.source_i.node2 != 0 ){
 				/* <-> */
 				value  = gsl_vector_get(tmp_vector , curr->node.source_i.node2 - 1 );
 				value += current;
 				gsl_vector_set(tmp_vector , curr->node.source_i.node2 - 1 , value);
 			}
 		}
 		/*
 		 * VOLTAGE SOURCE
 		 */
 		else if ( curr->type == NODE_SOURCE_V_TYPE ){
 			m2_elements_found++;
 			int matrix_row = list->hashtable->num_nodes  + m2_elements_found - 1 ;

 			double value;
 			/* set vector value */
 			value = gsl_vector_get(tmp_vector , matrix_row  );
 			value += curr->node.source_v.value;
 			gsl_vector_set(tmp_vector, matrix_row , value);

 			/* Change the matrix */
 			int plus_node  = curr->node.source_v.node1 - 1 ;
 			int minus_node = curr->node.source_v.node2 - 1;

 			/* <+> */
 			if( plus_node != -1 ){

 				value = gsl_matrix_get(tmp_matrix , matrix_row , plus_node);
 				value++;
 				gsl_matrix_set(tmp_matrix , matrix_row , plus_node , value);
 				printf("VOLTAGE SOURCE : (%d,%d) +1\n",matrix_row,plus_node);

 				value = gsl_matrix_get(tmp_matrix , plus_node , matrix_row);
 				value++;
 				gsl_matrix_set(tmp_matrix , plus_node , matrix_row , value); 
 				printf("VOLTAGE SOURCE : (%d,%d) + 1 \n",plus_node,matrix_row);				
 			} 
 			/* <->*/
 			if( minus_node != -1 ){
 				value = gsl_matrix_get(tmp_matrix , matrix_row , minus_node);
 				value++;
 				gsl_matrix_set(tmp_matrix , matrix_row , minus_node , value);

 				value = gsl_matrix_get(tmp_matrix , minus_node , matrix_row);
 				value++;
 				gsl_matrix_set(tmp_matrix , minus_node , matrix_row , value);
 			}
 		}
 		/*
 		 * Inductance
 		 */
 		else if ( curr->type == NODE_INDUCTANCE_TYPE ){
 			m2_elements_found++;
 			int matrix_row = list->hashtable->num_nodes  + m2_elements_found - 1 ;

 			double value;
 			/* set vector value */
 			//value = gsl_vector_get(tmp_vector , matrix_row  );
 			///value += curr->node.source_v.value;
 			//gsl_vector_set(tmp_vector, matrix_row , value);

 			/* Change the matrix */
 			int plus_node  = curr->node.inductance.node1 - 1 ;
 			int minus_node = curr->node.inductance.node2 - 1;

 			/* <+> */
 			if( plus_node != -1 ){

 				value = gsl_matrix_get(tmp_matrix , matrix_row , plus_node);
 				value++;
 				gsl_matrix_set(tmp_matrix , matrix_row , plus_node , value);
 				

 				value = gsl_matrix_get(tmp_matrix , plus_node , matrix_row);
 				value++;
 				gsl_matrix_set(tmp_matrix , plus_node , matrix_row , value); 
 								
 			} 
 			/* <->*/
 			if( minus_node != -1 ){
 				value = gsl_matrix_get(tmp_matrix , matrix_row , minus_node);
 				value++;
 				gsl_matrix_set(tmp_matrix , matrix_row , minus_node , value);

 				value = gsl_matrix_get(tmp_matrix , minus_node , matrix_row);
 				value++;
 				gsl_matrix_set(tmp_matrix , minus_node , matrix_row , value);
 			}
 		}
 	}

 	*matrix = tmp_matrix;
 	*vector = tmp_vector;
 	/* return */
 	return 1;
}
