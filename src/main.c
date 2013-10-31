#include <stdio.h>
#include <string.h>
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
 		printf("ERROR READING NETLIST\n");
 	}


 	print_list(list);
 	printf("List length = %d \n",list.len);
 	free_list(&list);

	return 0;
}
