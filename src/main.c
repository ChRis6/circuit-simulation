#include <stdio.h>
#include <string.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_linalg.h>


#include "circuit_sim.h"
#include "node_list.h"

#include "parse.h"
#include "circuit_hash.h"
#include "iter_solve.h"
#include "decomposition.h"
#include "sparse_interface.h"
#include "circuit_sim_sparse.h"
#include "plot.h"

int main( int argc , char* argv[]){
	int flag;
	
	gsl_matrix *matrix;
	gsl_vector *vector;
	gsl_vector *x;
	gsl_permutation* permutation;

	int sign;

	if( argc < 2 ){
		printf("Usage: %s <netlist>\n",argv[0]);
		return -1;
	}

	printf("Starting simulation....\n");

 	LIST list;
 	init_list(&list);
 	
 	//FILE *input_file=fopen(argv[1], "r");
 	//int flag = check_netlist_file(input_file,&list);
 	flag = parse_netlist(argv[1] , &list);
 	if( !flag ){
 		printf("ERROR BUILDING NETLIST\n");
 		free_list(&list);
 		return -1;
 	}

 	int i,j;
 	sparse_matrix* s_matrix;
 	sparse_vector* s_vector;
 	int size;
 	create_mna(&list,&matrix,&vector);

 	s_matrix = create_mna_sparse(&list,&s_vector,&size);
 	printf("Printing vectors: \n");
 	for( i = 0 ; i < size ; i++)
 		printf("vector = %f s_vector = %f \n", gsl_vector_get(vector,i) , s_vector[i] );

 	cs_print(s_matrix , "MATRIX_SPARSE.txt",0);
 	for( i = 0 ; i < matrix->size1 ; i++)
 		for( j = 0 ; j < matrix->size2; j++)
 			if( gsl_matrix_get(matrix,i,j) != 0)
 				fprintf(stderr, "(%d,%d) = %f\n", i , j , gsl_matrix_get(matrix,i,j) );


 	printf("Solving Method = %d\n",list.solving_method);
 	if ( !list.sparse ){


 		flag = create_mna(&list, &matrix, &vector );
 		if(!flag ){
 			printf("Error creating mna system\n");
 			return -1;
 		}


		x = gsl_vector_calloc(matrix->size1);
 		if( !x ){
 			printf("X vector : no memory\n");
 			exit(1);
 		}
	
		/* Cholesky or LU */
		if( list.solving_method == METHOD_LU || list.solving_method == METHOD_CHOLESKY ){

 			decomposition(matrix,&permutation,&sign,list.solving_method);
 	
 			if(list.dc_sweep.node != NULL)
 			{
 	 			dc_sweep(list,matrix,vector,x,permutation,list.solving_method);
 			}
 			else
 			{
 				int array_size = 1;
 				solve(matrix,vector,x,permutation,list.solving_method);
 				if(list.plot == PLOT_ON)
				{
 					gsl_vector ** plot_array;

					plot_array = plot_create_vector( array_size , x->size);
					if(plot_array == NULL)
				{
					perror("Error while allocating the ploting array\n");
					exit(0);
				}
	 		
				plot_set_vector_index(plot_array ,x ,0);
			 		 	
				plot_to_file(list.hashtable,plot_array,array_size,"results_plot_file.txt");
				}
 			}
		}
		else if ( list.solving_method == METHOD_CG ){
			printf("Solving using CG...\n");

			if( list.dc_sweep.node != NULL ){
				dc_sweep(list,matrix,vector,x,permutation,list.solving_method);
			}
			else {
				iter_solve_cg( matrix , vector , x);


				gsl_vector ** plot_array;

				plot_array = plot_create_vector( 1 , x->size);
				if(plot_array == NULL)
				{
					perror("Error while allocating the ploting array\n");
					exit(0);
				}
	 		
				plot_set_vector_index(plot_array ,x ,0);
			 		 	
				plot_to_file(list.hashtable,plot_array,1  ,"results_plot_file_cg.txt");
			}
		}
		else if( list.solving_method == METHOD_BICG){
			printf("Solving using BICG...\n");
			if( list.dc_sweep.node != NULL ){
				dc_sweep(list,matrix,vector,x,permutation,list.solving_method);
			}
			else {
				iter_solve_bicg( matrix , vector , x);


				gsl_vector ** plot_array;

				plot_array = plot_create_vector( 1 , x->size);
				if(plot_array == NULL)
				{
					perror("Error while allocating the ploting array\n");
					exit(0);
				}
	 		
				plot_set_vector_index(plot_array ,x ,0);
			 		 	
				plot_to_file(list.hashtable,plot_array,1  ,"results_plot_file_bicg.txt");
			}

		}
 	}
 	else {			//  sparse simulation
 		sparse_matrix* matrix;
 		sparse_vector* vector;
 		sparse_vector* x;
 		int vector_size;
 		char method;
 		int i;

 		method = list.solving_method;
 		matrix = (sparse_matrix*)create_mna_sparse( &list , &vector , &vector_size);
 		if( !matrix ){
 			fprintf(stderr, "Error creating MNA matrix \n");
 			exit(1);
 		}

 		x = (sparse_vector*) malloc( vector_size * sizeof(sparse_vector));


 		if( method == METHOD_LU_SPARSE ){
 			if( !sparse_solve_LU( matrix,vector,x,vector_size) ){
 				fprintf(stderr, "Solving Method Sparse LU failed\n" );
 			}

 		}
 		else if( method == METHOD_CHOLESKY_SPARSE ){
			if( !sparse_solve_cholesky( matrix,vector,x,vector_size) ){
 				fprintf(stderr, "Solving Method Sparse Cholesky failed\n" );
 			}

 		}
 		else if ( method == METHOD_CG_SPARSE ){


 		}
 		else if( method == METHOD_BICG_SPARSE ){


 		}
 		else{
 			fprintf(stderr, "Solving method not specified\n");
 		}

 		fprintf(stderr, "PRINTING SOLUTION FOUND:\n");
 		for( i = 0 ; i < vector_size ; i++)
 			fprintf(stderr, "%f\n", x[i]);
 		fprintf(stderr, "\n");

 		/* clean up before exit */
 		cs_spfree(matrix);
 		free(vector);
 		free(x);
 	}

 	 	
 


/*
 * Clean up
 */
 	free_list(&list);

 	if( !list.sparse){
 		gsl_vector_free(vector);
 		gsl_matrix_free(matrix);
 		gsl_permutation_free(permutation);
 	}

	return 0;
}
