#include <stdio.h>
#include <string.h>
#include "node_list.h"

#include "parse.h"

int main( int argc , char* argv[]){
	 
 	LIST list;
 	init_list(&list);
 	
 	FILE *input_file=fopen(argv[1], "r");
 	int flag = check_netlist_file(input_file,&list);

 	print_list(list);

 	free_list(&list);

	return 0;
}
