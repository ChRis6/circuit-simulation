#include <stdio.h>
#include <string.h>
#include "node_list.h"

#include "parse.h"

int main( int argc , char* argv[]){
	 
	if( argc < 2 ){
		printf("Usage: %s <netlist>\n",argv[0]);
		return -1;
	}
 	LIST list;
 	init_list(&list);
 	
 	FILE *input_file=fopen(argv[1], "r");
 	int flag = check_netlist_file(input_file,&list);


 	print_list(list);
 	printf("List length = %d \n",list.len);
 	free_list(&list);
 	fclose(input_file);
	return 0;
}
