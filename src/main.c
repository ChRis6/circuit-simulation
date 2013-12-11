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

 	flag = create_mna(&list, &matrix, &vector );
 	if(!flag ){
 		printf("Error creating mna system\n");
 		return -1;
 	}
 	
 	/*
 	 * Print mna matrix
 	 */
 	gsl_matrix_fprintf(stderr,matrix,"%f");



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
 		}else
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
 	 	



 
/*
 * Printing options after the simulation is over
 */
	int choice;
	fprintf(stderr, "Choose of the available options\n");
	while (1){
		fprintf(stderr, "1) Print MNA matrix\n" );
		fprintf(stderr, "2) Quit\n");
		scanf("%d",&choice);
		if( choice == 2 )
			break;


		gsl_matrix_fprintf(stderr,matrix,"%f");
		fprintf(stderr, "\n");
	}




/*
 * Clean up
 */
 	free_list(&list);
 	gsl_vector_free(vector);
 	gsl_matrix_free(matrix);
 	
	return 0;
}
