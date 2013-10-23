#include <stdio.h>
#include <string.h>
#include "node_list.h"

int main( int argc , char* argv[]){
	
	printf("Hello world\n");
 
 	LIST list;
 	RCL_T resistance;

 	SOURCE_V_T voltage;
 	init_list(&list);
 	

 	strcpy( voltage.name , "V1");
 	strcpy( voltage.node1 , "1");
 	strcpy( voltage.node2 , "0");
 	voltage.value = 10.0;

 	strcpy( resistance.name , "R1");
 	strcpy( resistance.node1 , "1");
 	strcpy( resistance.node2 , "2");
 	resistance.value = 5.0;

 	printf("Adding to list\n");

 	if( !add_to_list(&list , NODE_RCL_TYPE , &resistance , sizeof(RCL_T) ) ){
 		printf("LIST ADD FAILED\n");
 		return -1;
 	}


 	strcpy( resistance.name , "R2");
 	strcpy( resistance.node1 , "1");
 	strcpy( resistance.node2 , "0");
 	resistance.value = 100.0;
 	
 	if( !add_to_list(&list , NODE_RCL_TYPE , &resistance , sizeof(RCL_T) ) ){
 		printf("LIST ADD FAILED\n");
 		return -1;
 	}

 	 if( !add_to_list(&list , NODE_SOURCE_V_TYPE , &voltage , sizeof(SOURCE_V_T) ) ){
 		printf("LIST ADD FAILED\n");
 		return -1;
 	}
 	 	printf("Added to list\n");


 	print_list(list);

 	free_list(&list);
 	print_list(list);

	return 0;
}
