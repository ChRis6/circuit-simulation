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
		
 	decomposition(matrix,&permutation,&x,&sign,list.solving_method);
 	
 	if(list.dc_sweep.node != NULL)
 	{
 	 	dc_sweep(list,matrix,vector,x,permutation);
 	}else
 	{
 		int array_size = 1;
 		solve(matrix,vector,x,permutation);
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

 	
 	
 	ht_print(list.hashtable);
 	free_list(&list);
 
	return 0;
}