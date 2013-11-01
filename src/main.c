#include <stdio.h>
#include <string.h>
#include "node_list.h"

#include "parse.h"
#include "circuit_hash.h"

int main( int argc , char* argv[]){
	int flag;
	hashtable_t *hashtable = NULL;
	

	hashtable =  ht_init( 10000 );
	if( !hashtable ){
		printf("hashtable failed\n");
		return -1;
	}

	flag = ht_insert_pair(hashtable , "abcd" , 1 );
	if( flag == 0){
		printf("abcd not inserted\n");
		return -1;
	}

	flag = ht_insert_pair(hashtable , "abcdefg" , 2 );
	if( flag == 0){
		printf("abcdefg not inserted\n");
		return -1;
	}

	flag = ht_insert_pair(hashtable , "xyz" , 3 );
	if( flag == 0){
		printf("xyz not inserted\n");
		return -1;
	}

	ht_get(hashtable,"abcd" , &flag );
	printf("Hash table : \"abcd\" = %d\n", flag);

	ht_get(hashtable,"abcdefg" , &flag );	
	printf("Hash table : \"abcdefg\" = %d\n", flag );

	ht_get(hashtable,"xyz" , &flag );
	printf("Hash table : \"xyz\" = %d\n",flag);

	if( ht_get(hashtable,"malakia",&flag) == 0 ){
		printf("Hash table \"malakia\" den uparxei\n");
	}

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
