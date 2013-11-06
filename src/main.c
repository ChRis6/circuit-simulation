#include <stdio.h>
#include <string.h>
#include <gsl/gsl_sf_bessel.h>


#include "node_list.h"

#include "parse.h"
#include "circuit_hash.h"

int main( int argc , char* argv[]){
	int flag;
 	

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

 	print_list(list);
 	printf("List length = %d \n",list.len);
 	
 	ht_print(list.hashtable);
 	free_list(&list);
 
	return 0;
}
