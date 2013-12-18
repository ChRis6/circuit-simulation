#include <stdlib.h>
#include <string.h>
#include "circuit_sim_sparse.h"

#define DEFAULT_NZ 15

sparse_matrix* create_mna_sparse(LIST *list, sparse_vector** b, int* vector_len ){

	int rows;
	int columns;
	sparse_vector* vector = NULL;
	sparse_matrix* matrix = NULL;
	LIST_NODE* curr;
	

	int m2_elements_found = 0;       // # of elements in group 2

	/* allocate matrix and vector */
	rows    = list->hashtable->num_nodes + list->m2;
 	columns = list->hashtable->num_nodes + list->m2;

 	matrix =  cs_spalloc( rows , columns , DEFAULT_NZ , 1 , 1 );
 	if(!matrix)
 		return NULL;

 	vector = (sparse_vector*) malloc( sizeof(sparse_vector) * rows);
 	if( !vector){
 		cs_spfree(matrix);
 		return NULL;
 	} 		

 	for( curr = list->head ; curr; curr = curr->next){

 		/*
 		 * RESISTANCE ELEMENT
 		 */

 		if( curr->type == NODE_RESISTANCE_TYPE ){

  			double conductance = 1 / curr->node.resistance.value ;
 			int plus_node  = curr->node.resistance.node1 - 1 ;
 			int minus_node = curr->node.resistance.node2  - 1;
 			
 			/* <+> is ground */
 		 	if( plus_node == -1 ){

 				//double value = gsl_matrix_get(tmp_matrix , minus_node , minus_node);
 				//value += conductance ; 
 				//gsl_matrix_set( tmp_matrix , minus_node , minus_node ,  value );
 				//printf("Adding to matrix element (%d,%d) value:%f\n\n",minus_node,minus_node,value);
 				if( !cs_entry(matrix, minus_node , minus_node , conductance) ){
 					fprintf(stderr, "Error while inserting element in sparse matrix\n");
 					free(vector);
 					cs_spfree(matrix);
 					return NULL;
 				}
 			}
 			/* <-> is ground */
 			else if ( minus_node == -1  ){

 				if( !cs_entry(matrix, plus_node , plus_node , conductance) ){
 					fprintf(stderr, "Error while inserting element in sparse matrix\n");
 					free(vector);
 					cs_spfree(matrix);
 					return NULL;
 				}
 			}
 			else {

 				/* <+> <+> */
 				if( !cs_entry(matrix, plus_node , plus_node , conductance) ){
 					fprintf(stderr, "Error while inserting element in sparse matrix\n");
 					free(vector);
 					cs_spfree(matrix);
 					return NULL;
 				}

 				/* <+> <-> */
 				if( !cs_entry(matrix, plus_node , minus_node , -conductance) ){
 					fprintf(stderr, "Error while inserting element in sparse matrix\n");
 					free(vector);
 					cs_spfree(matrix);
 					return NULL;
 				}

 				/* <-> <+> */
 				if( !cs_entry(matrix, minus_node , plus_node , -conductance) ){
 					fprintf(stderr, "Error while inserting element in sparse matrix\n");
 					free(vector);
 					cs_spfree(matrix);
 					return NULL;
 				}

 				/* <-> <-> */
 				if( !cs_entry(matrix, minus_node , minus_node , conductance) ){
 					fprintf(stderr, "Error while inserting element in sparse matrix\n");
 					free(vector);
 					cs_spfree(matrix);
 					return NULL;
 				}
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
 				value  = vector[curr->node.source_i.node1 - 1 ];
 				value -= current;
 				vector[curr->node.source_i.node1 -1 ] =  value;
 			}

 			if( curr->node.source_i.node2 != 0 ){
 				/* <-> */
 				value  = vector[curr->node.source_i.node2 - 1 ];
 				value += current;
 				vector[curr->node.source_i.node2 -1 ] =  value;
 			}
 		}
 	 	/*
 		 * VOLTAGE SOURCE
 		 */
 		else if ( curr->type == NODE_SOURCE_V_TYPE  ){
 			m2_elements_found++;
 			int matrix_row = list->hashtable->num_nodes + m2_elements_found - 1;
 			curr->node.source_v.mna_row = matrix_row;

 			double value;

 			/* set vector value */
 			value = vector[matrix_row];
 			value += curr->node.source_v.value;
 			vector[ matrix_row ] = value;

 			int plus_node  = curr->node.source_v.node1 - 1;
 			int minus_node = curr->node.source_v.node2 - 1;

 			/* <+> */
 			if( plus_node != -1 ){

 				if( !cs_entry(matrix, matrix_row , plus_node , 1.0 ) ){
 		 			fprintf(stderr, "Error while inserting element in sparse matrix\n");
 					free(vector);
 					cs_spfree(matrix);
 					return NULL;	
 				}

 				if( !cs_entry(matrix, plus_node , matrix_row , 1.0 ) ){
 		 			fprintf(stderr, "Error while inserting element in sparse matrix\n");
 					free(vector);
 					cs_spfree(matrix);
 					return NULL;	
 				}
 			}

 			/* <-> */
 			if( minus_node != -1 ){
 				if( !cs_entry(matrix, matrix_row , minus_node , 1.0 ) ){
 		 			fprintf(stderr, "Error while inserting element in sparse matrix\n");
 					free(vector);
 					cs_spfree(matrix);
 					return NULL;	
 				}
 				if( !cs_entry(matrix, minus_node , matrix_row , -1.0 ) ){
 		 			fprintf(stderr, "Error while inserting element in sparse matrix\n");
 					free(vector);
 					cs_spfree(matrix);
 					return NULL;	
 				}
 			}	 
 		}

 		/*
 		 * Inductance
 		 */
 		else if ( curr->type == NODE_INDUCTANCE_TYPE  ){
 			m2_elements_found++;
 			int matrix_row = list->hashtable->num_nodes  + m2_elements_found - 1 ;
		
 			/* Change the matrix */
 			int plus_node  = curr->node.inductance.node1 - 1;
 			int minus_node = curr->node.inductance.node2 - 1;
 			/* <+> */
 			if( plus_node != -1 ){

 				if( !cs_entry(matrix, matrix_row , plus_node , 1.0 ) ){
 		 			fprintf(stderr, "Error while inserting element in sparse matrix\n");
 					free(vector);
 					cs_spfree(matrix);
 					return NULL;	
 				}

 				if( !cs_entry(matrix, plus_node , matrix_row , 1.0 ) ){
 		 			fprintf(stderr, "Error while inserting element in sparse matrix\n");
 					free(vector);
 					cs_spfree(matrix);
 					return NULL;	
 				}
 			}

 			/* <-> */
 			if( minus_node != -1 ){
 				if( !cs_entry(matrix, matrix_row , minus_node , 1.0 ) ){
 		 			fprintf(stderr, "Error while inserting element in sparse matrix\n");
 					free(vector);
 					cs_spfree(matrix);
 					return NULL;	
 				}
 				if( !cs_entry(matrix, minus_node , matrix_row , -1.0 ) ){
 		 			fprintf(stderr, "Error while inserting element in sparse matrix\n");
 					free(vector);
 					cs_spfree(matrix);
 					return NULL;	
 				}
 			}
 		}
 	}

	matrix = cs_compress(matrix);
 	/* remove duplicates */
 	
 	if( !cs_dupl(matrix) ){
 		fprintf(stderr, "Sparse matrix: duplicates not removed \n");
 		cs_spfree(matrix);
 		free(vector);
 		return NULL;
 	}
	
 	*b = vector;
 	*vector_len = rows;
 	return matrix;
}


int sparse_LU_decomp(sparse_matrix* matrix, css* S, csn* N ){

	if( !matrix)
		return 0;

	S=cs_sqr(2,matrix,0);
	N=cs_lu(matrix,S,1);
	
	if(!S || !N)
		return 0;

	cs_spfree(matrix);

	return 1;
}

int sparse_solve_LU(sparse_matrix* matrix, sparse_vector* b, sparse_vector* x, int n){

	if( n < 1 )
		return 0;

	memcpy(x,b ,n * sizeof(double));

	return cs_lusol(2 , matrix , x , 1.0 );

}

int sparse_solve_cholesky(sparse_matrix* matrix, sparse_vector* b, sparse_vector* x, int n){
		
		if( n < 1)
			return 0;
	memcpy(x,b,n*sizeof(double));

	return cs_cholsol(1 , matrix , x);
}
