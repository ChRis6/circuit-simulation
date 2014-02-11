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
#include "iter_solve_sparse.h"
#include "decomposition.h"
#include "sparse_interface.h"
#include "circuit_sim_sparse.h"
#include "plot.h"

static void print_gsl_vector(gsl_vector* vector){
	int i;
	for(i = 0; i < vector->size; i++)
		fprintf(stderr, "%f\n", gsl_vector_get(vector,i));

}

static void double_vector_to_gsl_vector(gsl_vector* gsl_v,double *doubl_vec,int size_n)
{
	int i;
	for (i = 0; i < size_n; i++)
	{
		gsl_vector_set(gsl_v,i,doubl_vec[i]);
	}
}
int main( int argc , char* argv[]){
	int flag;
	
	gsl_matrix *matrix;
	gsl_matrix *c_matrix;
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

 	printf("Solving Method = %s\n",solving_method_names[list.solving_method-1]);
 	if ( !list.sparse ){


 		flag = create_mna(&list, &matrix, &vector, !TRANSIENT,&c_matrix);
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
					fprintf(stderr, "Priting solution found...\n");
					print_gsl_vector(x);
			 		if ( list.solving_method == METHOD_LU )
						plot_to_file(list.hashtable,plot_array,array_size,"results_plot_file_lu.txt");
					else
						plot_to_file(list.hashtable,plot_array,array_size,"results_plot_file_chol.txt");
				}
 			}
		}
		else if ( list.solving_method == METHOD_CG ){
			//printf("Solving using CG...\n");

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
			 	fprintf(stderr, "Priting solution found...\n");
			 	print_gsl_vector(x);
				plot_to_file(list.hashtable,plot_array,1  ,"results_plot_file_cg.txt");
			}
		}
		else if( list.solving_method == METHOD_BICG){
			//printf("Solving using BICG...\n");
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
			 	fprintf(stderr, "Priting solution found...\n");
			 	print_gsl_vector(x);
				plot_to_file(list.hashtable,plot_array,1  ,"results_plot_file_bicg.txt");
			}

		}
 	}
 	else {
 		//  sparse simulation
 		int vector_size;
 		char method;
 		int i;
 		sparse_matrix* matrix;
 		sparse_vector* vector;
 		sparse_vector* x;
 		gsl_vector* x_sparse;
		gsl_vector* vector_sparse;
		gsl_vector* vector_gsl;

 		

 		method = list.solving_method;
 		matrix = (sparse_matrix*)create_mna_sparse( &list , &vector , &vector_size);
 		if( !matrix ){
 			fprintf(stderr, "Error creating MNA matrix \n");
 			exit(1);
 		}
 		
 		/* Added by hriskons */
 		/* conversion of a double into a gsl */
 		x_sparse = gsl_vector_calloc(matrix->n);
		vector_sparse = gsl_vector_calloc(matrix->n);
 		double_vector_to_gsl_vector(vector_sparse,vector,vector_size);

 		/* print sparse matrix */
 		//cs_print(matrix,"sparse_matrix.txt",0);

 		x = (sparse_vector*) malloc( vector_size * sizeof(sparse_vector));


 		if( method == METHOD_LU_SPARSE ){
 			if( !sparse_solve_LU( matrix,vector,x,vector_size) ){
 				fprintf(stderr, "Solving Method Sparse LU failed\n" );
 			}

 			gsl_vector ** plot_array;

			plot_array = plot_create_vector( 1 , vector_size);
			if(plot_array == NULL)
			{
				perror("Error while allocating the ploting array\n");
				exit(0);
			}
	 		vector_gsl = gsl_vector_calloc(vector_size);
 			double_vector_to_gsl_vector(vector_gsl,x,vector_size);

			plot_set_vector_index(plot_array ,vector_gsl ,0);
			 		 	
			plot_to_file(list.hashtable,plot_array,1  ,"results_plot_file_lu_sparse.txt");


 		}
 		else if( method == METHOD_CHOLESKY_SPARSE ){
			if( !sparse_solve_cholesky( matrix,vector,x,vector_size) ){
 				fprintf(stderr, "Solving Method Sparse Cholesky failed\n" );
 			}

 		}	
 		else if ( method == METHOD_CG_SPARSE ){
 			if( !sparse_solve_cg( matrix,vector_sparse,x_sparse) ){
 				fprintf(stderr, "Solving Method Sparse CG failed\n" );
 			}
 			gsl_vector ** plot_array;

			plot_array = plot_create_vector( 1 , x_sparse->size);
			if(plot_array == NULL)
			{
				perror("Error while allocating the ploting array\n");
				exit(0);
			}
	 		
			plot_set_vector_index(plot_array ,x_sparse ,0);
			 		 	
			plot_to_file(list.hashtable,plot_array,1  ,"results_plot_file_sparse_cg.txt");
 		}
 		else if( method == METHOD_BICG_SPARSE ){
			if( !sparse_solve_bicg( matrix, vector_sparse, x_sparse) ){
 				fprintf(stderr, "Solving Method Sparse BiCG failed\n" );
 			}
 			gsl_vector ** plot_array;

			plot_array = plot_create_vector( 1 , x_sparse->size);
			if(plot_array == NULL)
			{
				perror("Error while allocating the ploting array\n");
				exit(0);
			}
	 		
			plot_set_vector_index(plot_array ,x_sparse ,0);
			 		 	
			plot_to_file(list.hashtable,plot_array,1  ,"results_plot_file_sparse_bicg.txt");
 

 		}
 		else{
 			fprintf(stderr, "Solving method not specified\n");
 		}

 		//fprintf(stderr, "PRINTING SOLUTION FOUND:\n");
 		//for( i = 0 ; i < vector_size ; i++)
 		//	fprintf(stderr, "%f\n", x[i]);
 		//fprintf(stderr, "\n");

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
 		//gsl_permutation_free(permutation);
 	}

	return 0;
}
