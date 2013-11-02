/*Small code for reading the netlist file line by line,
 extracting string tokens from each line,
 and recognizing the type of element described on each line.
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"


#define	RESISTANCE_NUM_PARSE_ELEMENTS  4
#define	CAPACITY_NUM_PARSE_ELEMENTS    4
#define INDUCTANCE_NUM_PARSE_ELEMENTS  4
#define MOSFET_NUM_PARSE_ELEMENTS      6 


static int get_node_from_line( LIST* list,char* line , NODE* node , int* type);

int check_netlist_file(FILE *input_file , LIST* list){

   int line_number=0;
   char *next_field=NULL; //ptr to the next field extracted from the current line
   char line[MAX_LINE_SIZE+1];
   char first_letter;

   int counter ;

   int found_zero = 0;
    while (1) {
      

      fgets(line, MAX_LINE_SIZE, input_file); // read the line from input_file

      // if we've reached the end of the input_file, break out of the loop
      if (feof(input_file))  
        break;

      // increment the line number because we are starting a new line
      line_number++;
      counter = 0 ;

      //check if the first character is a valid element or comment 
      first_letter=line[0];
      if((first_letter=='V')||(first_letter=='v'))
      {
 	      //printf("To %do stoixeio Einai Phgh tashs\n", line_number);     
        
        SOURCE_V_T voltage;

        /* get name */
        next_field = strtok(line, " ");
        strcpy( voltage.name , next_field);
        //must implement field number testing for invalid entries in netlist file (more or less fields than expected)
        while ((next_field != NULL) && (first_letter!='*')) {
        
          // write the current field to the output file
          //printf("%s\n",next_field);

        // get the next word on the line--remember to pass NULL as the first argument
        // because only the first call to strtok on each line should receive a pointer
        // to line
          next_field = strtok(NULL, " \n");
          counter++;
          if( counter == 1 ){
            int node1 = atoi( next_field);
            voltage.node1 = node1;

            if( node1 == 0)
              found_zero = 1;
          }
          else if( counter == 2){
            int node2 = atoi( next_field);
            voltage.node2 = node2;
            if( node2 == 0)
              found_zero = 1;         
          }
          else if( counter == 3){
            sscanf(next_field , "%lf",&(voltage.value));
          }
          else if( counter > 4 ) {
            /* INPUT FILE has errors*/
            printf("Error while parsing input file...\n");
            exit(1);
          }
        }
        /* add voltage source to circuit node list */
        add_to_list( list , NODE_SOURCE_V_TYPE , &voltage , sizeof(SOURCE_V_T));

      }
      else if((first_letter=='I')||(first_letter=='i'))
      {
 	      //printf("To %do stoixeio Einai Phgh revmatos\n", line_number);     
        SOURCE_I_T source_i;

        /* get name */
        next_field = strtok(line, " \n");
        strcpy( source_i.name , next_field);

        //printf("line: %d next_field: %s\n",line_number,next_field);
        while ((next_field != NULL) && (first_letter!='*')) {

          next_field = strtok(NULL, " \n");
          counter++;
          if( counter == 1 ){
            int node1 = atoi( next_field);
            source_i.node1 = node1;

            if( node1 == 0)
              found_zero = 1;
          }
          else if( counter == 2){
            int node2 = atoi( next_field);
            source_i.node2 = node2;
            //printf("line: %d next_field: %s\n node2: %d",line_number,next_field,source_i.node2);

            if( node2 == 0)
              found_zero = 1;
          }
          else if( counter == 3){
            sscanf(next_field , "%lf",&(source_i.value));
            //printf("line: %d next_field: %s\n",line_number,next_field);
          }
          else if( counter > 4 ) {
            /* INPUT FILE has errors*/
            printf("Error while parsing input file...\n");
            exit(1);
          }
        }

        /* add current source to node list */
        add_to_list( list , NODE_SOURCE_I_TYPE , &source_i , sizeof(SOURCE_I_T));
  
      }
      else if((first_letter == 'C') || (first_letter=='c') )
      {
 	      //printf("To %do stoixeio Einai puknwths\n", line_number);     
        //printf("capacity found\n");
        CAPACITY_T cap ;

        /* get name */
        next_field = strtok(line, " \n");
        strcpy( cap.name , next_field);

        while ((next_field != NULL) && (first_letter!='*')) {
          
          next_field = strtok(NULL, " \n");
          counter++;
          if( counter == 1 ){
            int node1 = atoi( next_field);
            cap.node1 = node1;

            //printf("line: %d next_field: %s node1: %d\n",line_number,next_field,cap.node1);
            if( node1 == 0)
              found_zero = 1;
          }
          else if( counter == 2){
            int node2 = atoi( next_field);
            cap.node2 = node2;
            //printf("line: %d next_field: %s node2: %d\n",line_number,next_field,cap.node2);

            if( node2 == 0)
              found_zero = 1;
          }
          else if( counter == 3){
            sscanf(next_field , "%lf",&(cap.value));
            //printf("line: %d next_field: %s\n",line_number,next_field);
          }
          else if( counter > 4 ) {
            /* INPUT FILE has errors*/
            printf("Error while parsing input file...\n");
            exit(1);
          }
        }
        add_to_list( list , NODE_CAPACITY_TYPE , &cap , sizeof(CAPACITY_T));
      }
     else if((first_letter =='R') || ( first_letter =='r') )
      {

        RESISTANCE_T resistor;
        /* get name */
        next_field = strtok(line, " \n");
        strcpy( resistor.name , next_field);

        while( (next_field != NULL) && ( first_letter != '*')){

          next_field = strtok(NULL , " \n");
          counter++;
          if(counter == 1 ){
            int node1 = atoi(next_field);
            resistor.node1 = node1;

            if( node1 == 0)
              found_zero = 1;
          }
          else if( counter == 2){

            int node2 = atoi(next_field);
            resistor.node2 = node2;

            if( node2 == 0 )
              found_zero =1;
          }
          else if( counter == 3 ){
            sscanf(next_field,"%lf",&(resistor.value));
          }
          else if( counter > 4){
            printf("Error while parsing input file\n");
            exit(1);
          }
        }
        add_to_list( list , NODE_RESISTANCE_TYPE , &resistor , sizeof(RESISTANCE_T));
      
      }
     else if((first_letter=='L')||(first_letter=='l') )
      {
        //printf("To %do stoixeio Einai antistash\n", line_number);     
        
        INDUCTANCE_T inductance ;

        /* get name */
        next_field = strtok(line, " \n");
        strcpy( inductance.name , next_field);

        while ((next_field != NULL) && (first_letter!='*')) {
          
          next_field = strtok(NULL, " \n");
          counter++;
          if( counter == 1 ){
            int node1 = atoi( next_field);
            inductance.node1 = node1;

            if( node1 == 0)
              found_zero = 1;
          }
          else if( counter == 2){
            int node2 = atoi( next_field);
            inductance.node2 = node2;
            //printf("line: %d next_field: %s\n node2: %d",line_number,next_field,source_i.node2);

            if( node2 == 0)
              found_zero = 1;
          }
          else if( counter == 3){
            sscanf(next_field , "%lf",&(inductance.value));
            //printf("line: %d next_field: %s\n",line_number,next_field);
          }
          else if( counter > 4 ) {
            /* INPUT FILE has errors*/
            printf("Error while parsing input file...\n");
            exit(1);
          }
        }
        add_to_list( list , NODE_INDUCTANCE_TYPE , &inductance , sizeof(INDUCTANCE_T));
      }
      else if((first_letter=='M')||(first_letter=='m'))
      {
      	MOSFET_T mosfet;
      	 /* get name */
        next_field = strtok(line, " \n");
        strcpy( mosfet.name , next_field);
 	  	//printf("To %do stoixeio Einai pyknwths\n", line_number);     
		    while ((next_field != NULL) && (first_letter!='*')) {
          
          next_field = strtok(NULL, " \n");
          counter++;
          if( counter == 1 ){
            int drain = atoi( next_field);
            mosfet.drain = drain;

            if( drain == 0)
              found_zero = 1;
          }
          else if( counter == 2){
            int gate = atoi( next_field);
            mosfet.gate = gate;
            //printf("line: %d next_field: %s\n node2: %d",line_number,next_field,source_i.node2);

            if( gate == 0)
              found_zero = 1;
          }
          else if( counter == 3){
            int source =atoi(next_field);
            mosfet.source=source;
          }
          else if(counter == 4){
          	int body = atoi(next_field);
          	mosfet.body = body;
          }
          else if(counter == 5){
          	sscanf(next_field , "%lf",&(mosfet.l));
          
          }
          else if(counter == 6){
          	sscanf(next_field , "%lf",&(mosfet.w));
            
          }
          else if( counter > 7 ) {
            /* INPUT FILE has errors*/
            printf("Error while parsing input file...\n");
            exit(1);
          }
        }  
        add_to_list( list , NODE_MOSFET_TYPE , &mosfet , sizeof(MOSFET_T));
      }
      else if(first_letter == 'Q' || first_letter == 'q'){
      	BJT_T bjt;

        /* get name */
        next_field = strtok(line, " \n");
        strcpy( bjt.name , next_field);

        while ((next_field != NULL) && (first_letter!='*')) {
          
          next_field = strtok(NULL, " \n");
          counter++;
          if( counter == 1 ){
            int collector = atoi( next_field);
            bjt.collector = collector;

            if( collector == 0)
              found_zero = 1;
            }
          else if( counter == 2){
            int base = atoi( next_field);
            bjt.base = base;
            //printf("line: %d next_field: %s\n node2: %d",line_number,next_field,source_i.node2);

            if( base == 0)
              found_zero = 1;
          }
          else if( counter == 3){
            int emitter = atoi( next_field);
            bjt.emitter = emitter;
            //printf("line: %d next_field: %s\n node2: %d",line_number,next_field,source_i.node2);

            if( emitter == 0)
              found_zero = 1;
          }
          else if( counter > 4 ) {
            /* INPUT FILE has errors*/
            printf("Error while parsing input file...\n");
            exit(1);
          }
        }
        add_to_list( list , NODE_BJT_TYPE , &bjt , sizeof(BJT_T));
      }
      
      else if(first_letter=='*')
      {
        printf("H %d grammh einai sxolio\n", line_number);
      }
      else
      {
        printf("Your netlist file is invalid, please try again\n");
	     return(0);
      }
      // initialize next_field to point to the first field on the line
      //next_field = strtok(line, " \n");
      
      // iterate through the fields on the line until we have exhausted them. strtok
      // will return NULL when there are no more fields to be read
      
      //must implement field number testing for invalid entries in netlist file (more or less fields than expected)
     /* while ((next_field != NULL) && (first_letter!='*')) {
        // write the current field to the output file
        fprintf(output_file, "%d: %s\n", line_number, next_field);

        // get the next word on the line--remember to pass NULL as the first argument
        // because only the first call to strtok on each line should receive a pointer
        // to line
        next_field = strtok(NULL, " \n");
      }*/
  }
   
  /* ground found ? */
  if( found_zero )
    return 1;

  return 0;
}



/*
 * Parse netlist
 */
int parse_netlist(char* filename , LIST* list){

	char line[ MAX_LINE_SIZE + 1];
	NODE element_node;
	int element_type;
	int line_number;
	int res;

	if( !filename || !list  )
		return 0;

	FILE* file;
	file = fopen(filename,"r");
	if( !file )
		return 0;

	line_number = 1 ;
	/*Read until EOF */
	while( !feof(file)){

		/* Get a single line */
		if( fgets( line , MAX_LINE_SIZE , file) != NULL ) {

			/* check for comment,else process */
			if( line[0] != '*'){
				res = get_node_from_line( list, line , &element_node , &element_type);
				if( res ){

					/* add node read and store at list */
					//printf("NODE READ: %s , %d , %d , %g \n",element_node.resistance.name , element_node.resistance.node1 , element_node.resistance.node2 , element_node.resistance.value);
					res = add_node_to_list(list , &element_node , element_type);
					if( !res ){
						printf("NO MEMORY\n");
						return 0;
					}
				}
				else{

					/* Error while parsing line */
					fclose(file);
					printf("Error while parsing.Line %d : %s\n",line_number , line);
					return 0;
				}
			}

		}
		line_number++;
	}

	return 1;
}


/* 
 * Proccess a single line
 *
 * 1) Identify circuit element
 * 2) Check for errors
 * 3) Build circuit node
 *
 * Retuns: 1 when a node was identified correctly.Variables -node-  and -type- contain a circuit node
 *         0 when a parsing error occurs.Variables -node- and -type- values are not predicted
 */

static int get_node_from_line( LIST* list,char* line , NODE* node , int* type){

	char c;
	char* token;
  int flag;
  static int node_count = 1;


	if( line == NULL || node == NULL  || type == NULL )
		return 0;

  //printf("\nParsing line: %s\n",line);
	c = line[0];


	switch(c){
		case 'R':
		case 'r':{
			/* read name */
			token = strtok(line," ");
			if( token == NULL ){

				return 0 ;
			}
			strcpy( node->resistance.name , token);

			/* 
       *Read <+> node
       */
			token = strtok(NULL," ");
			if( token == NULL){
				return 0;
			}

      /* check for reference node (ground) */
      if( strcmp(token,"0") == 0 ){
        node->resistance.node1 = 0;
        list->has_reference = 1;
      }
      else{
        /*
         * this is not a reference node.Add string to
         * hash table
         */
        flag = ht_insert_pair(list->hashtable, token , node_count);
        if( flag == 1 ){
          /* successfull insertion */
          node->resistance.node1 = node_count;
          node_count++;    // get ready for the next node
        }
        else if( flag == 0 ){
          /* NULL pointer or out of memory */
          printf("Error at inserting pair to hash table..\n");
          //free_list(list);
          exit(1);
        }
        else if( flag == -1 ){

          int n;
          printf("Node : \"%s\" already on hash table \n",token);
          ht_get(list->hashtable,token,&n);
          node->resistance.node1 = n;
        }
      }
			

      /* 
       * Read <-> node
       */
			token = strtok(NULL," ");
			if( token == NULL){
				return 0;
			}


      /* check for reference node (ground) */
      if( strcmp(token,"0") == 0 ){
        node->resistance.node2 = 0;
        list->has_reference = 1;
      }
      else{
        /*
         * this is not a reference node.Add string to
         * hash table
         */
        flag = ht_insert_pair(list->hashtable, token , node_count);
        if( flag == 1 ){
          /* successfull insertion */
          node->resistance.node2 = node_count;
          node_count++;    // get ready for the next node
        }
        else if( flag == 0 ){
          /* NULL pointer or out of memory */
          printf("Error at inserting pair to hash table..\n");
          //free_list(list);
          exit(1);
        }
        else if( flag == -1 ){

          int n;
          printf("Node : \"%s\" already on hash table \n",token);
          ht_get(list->hashtable,token,&n);
          node->resistance.node2 = n;
        }
      }      	

      //node->resistance.node2 = atoi(token);

			/* read value node */
			token = strtok(NULL," ");
			if( token == NULL){
				return 0;
			}
    			
      node->resistance.value = atof(token);

			/* NO MORE TOKENS.IF FOUND RETURN ERROR */
			token = strtok(NULL," \n");
			if( token == NULL ){
				*type = NODE_RESISTANCE_TYPE;
				return 1;
			}
			else{
				/* tokens were found.print for debugging...*/
				printf("LINE: %s , garbage token : %s\n" , line , token);
				return 0;
			}

		}
		case 'C':
		case 'c':{
			/* read name */
			token = strtok(line," ");
			if( token == NULL ){
				return 0 ;
			}
			strcpy( node->capacity.name , token);			

			/* read <+> node */
			token = strtok(NULL," ");
			if( token == NULL){
				return 0;
			}

      /* check for reference node (ground) */
      if( strcmp(token,"0") == 0 ){
        node->capacity.node1 = 0;
        list->has_reference = 1;
      }
      else{
        /*
         * this is not a reference node.Add string to
         * hash table
         */
        flag = ht_insert_pair(list->hashtable, token , node_count);
        if( flag == 1 ){
          /* successfull insertion */
          node->capacity.node1 = node_count;
          node_count++;    // get ready for the next node
        }
        else if( flag == 0 ){
          /* NULL pointer or out of memory */
          printf("Error at inserting pair to hash table..\n");
          //free_list(list);
          exit(1);
        }
        else if( flag == -1 ){
          int n;
          printf("Node : \"%s\" already on hash table \n",token);
          ht_get(list->hashtable,token,&n);
          node->capacity.node1 = n;
        }
      } 

//			node->capacity.node1 = atoi(token);
			
			/* read <-> node */
			token = strtok(NULL," ");
			if( token == NULL){
				return 0;
			}

      /* check for reference node (ground) */
      if( strcmp(token,"0") == 0 ){
        node->capacity.node2 = 0;
        list->has_reference = 1;
      }
      else{
        /*
         * this is not a reference node.Add string to
         * hash table
         */
        flag = ht_insert_pair(list->hashtable, token , node_count);
        if( flag == 1 ){
          /* successfull insertion */
          node->capacity.node2 = node_count;
          node_count++;    // get ready for the next node
        }
        else if( flag == 0 ){
          /* NULL pointer or out of memory */
          printf("Error at inserting pair to hash table..\n");
          //free_list(list);
          exit(1);
        }
        else if( flag == -1 ){
          int n;
          printf("Node : \"%s\" already on hash table \n",token);
          ht_get(list->hashtable,token,&n);
          node->capacity.node2 = n;
        }
      } 

			/* read value node */
			token = strtok(NULL," ");
			if( token == NULL){
				return 0;
			}
			node->capacity.value = atof(token);

			/* NO MORE TOKENS.IF FOUND RETURN ERROR */
			token = strtok(NULL," \n");
			if( token == NULL ){
				*type = NODE_CAPACITY_TYPE;
				return 1;
			}
			else{
				/* tokens were found.print for debugging...*/
				printf("LINE: %s , garbage token : %s\n" , line , token);
				return 0;
			}
		}
		case 'L':
		case 'l':{

			/* read name */
			token = strtok(line," ");
			if( token == NULL ){
				return 0 ;
			}
			strcpy( node->inductance.name , token);
			
			/* read <+> node */
			token = strtok(NULL," ");
			if( token == NULL){
				return 0;
			}
      /* check for reference node (ground) */
      if( strcmp(token,"0") == 0 ){
        node->inductance.node1 = 0;
        list->has_reference = 1;
      }
      else{
        /*
         * this is not a reference node.Add string to
         * hash table
         */
        flag = ht_insert_pair(list->hashtable, token , node_count);
        if( flag == 1 ){
          /* successfull insertion */
          node->inductance.node1 = node_count;
          node_count++;    // get ready for the next node
        }
        else if( flag == 0 ){
          /* NULL pointer or out of memory */
          printf("Error at inserting pair to hash table..\n");
          //free_list(list);
          exit(1);
        }
        else if( flag == -1 ){
          int n;
          printf("Node : \"%s\" already on hash table \n",token);
          ht_get(list->hashtable,token,&n);
          node->inductance.node1 = n;
        }
      } 


//			node->inductance.node1 = atoi(token);
			
			/* read <-> node */
			token = strtok(NULL," ");
			if( token == NULL){
				return 0;
			}

      /* check for reference node (ground) */
      if( strcmp(token,"0") == 0 ){
        node->inductance.node2 = 0;
        list->has_reference = 1;
      }
      else{
        /*
         * this is not a reference node.Add string to
         * hash table
         */
        flag = ht_insert_pair(list->hashtable, token , node_count);
        if( flag == 1 ){
          /* successfull insertion */
          node->inductance.node2 = node_count;
          node_count++;    // get ready for the next node
        }
        else if( flag == 0 ){
          /* NULL pointer or out of memory */
          printf("Error at inserting pair to hash table..\n");
          //free_list(list);
          exit(1);
        }
        else if( flag == -1 ){
          int n;
          printf("Node : \"%s\" already on hash table \n",token);
          ht_get(list->hashtable,token,&n);
          node->inductance.node2 = n;
        }
      } 


			//node->inductance.node2 = atoi(token);

			/* read value node */
			token = strtok(NULL," ");
			if( token == NULL){
				return 0;
			}
			node->inductance.value = atof(token);

			/* NO MORE TOKENS.IF FOUND RETURN ERROR */
			token = strtok(NULL," \n");
			if( token == NULL ){
				*type = NODE_INDUCTANCE_TYPE;
				return 1;
			}
			else{
				/* tokens were found.print for debugging...*/
				printf("LINE: %s , garbage token: %s\n" , line , token);
				return 0;
			}			
		}

		/*
		 * VOLTAGE SOURCE
		 */
		case 'v':
		case 'V':{

			/* read name */
			token = strtok(line," ");
			if( token == NULL ){
				return 0 ;
			}
			strcpy( node->source_v.name , token);

			/* read <+> node */
			token = strtok(NULL," ");
			if( token == NULL){
				return 0;
			}

      /* check for reference node (ground) */
      if( strcmp(token,"0") == 0 ){
        node->source_v.node1 = 0;
        list->has_reference = 1;
      }
      else{
        /*
         * this is not a reference node.Add string to
         * hash table
         */
        flag = ht_insert_pair(list->hashtable, token , node_count);
        if( flag == 1 ){
          /* successfull insertion */
          node->source_v.node1 = node_count;
          node_count++;    // get ready for the next node
        }
        else if( flag == 0 ){
          /* NULL pointer or out of memory */
          printf("Error at inserting pair to hash table..\n");
          //free_list(list);
          exit(1);
        }
        else if( flag == -1 ){
          int n;
          printf("Node : \"%s\" already on hash table \n",token);
          ht_get(list->hashtable,token,&n);
          node->source_v.node1 = n;
        }
      } 


			//node->source_v.node1 = atoi(token);

			/* read <-> node */
			token = strtok(NULL," ");
			if( token == NULL){
				return 0;
			}

      /* check for reference node (ground) */
      if( strcmp(token,"0") == 0 ){
        node->source_v.node2 = 0;
        list->has_reference = 1;
      }
      else{
        /*
         * this is not a reference node.Add string to
         * hash table
         */
        flag = ht_insert_pair(list->hashtable, token , node_count);
        if( flag == 1 ){
          /* successfull insertion */
          node->source_v.node2 = node_count;
          node_count++;    // get ready for the next node
        }
        else if( flag == 0 ){
          /* NULL pointer or out of memory */
          printf("Error at inserting pair to hash table..\n");
          //free_list(list);
          exit(1);
        }
        else if( flag == -1 ){
          int n;
          printf("Node : \"%s\" already on hash table \n",token);
          ht_get(list->hashtable,token,&n);
          node->source_v.node2 = n;
        }
      } 

			//node->source_v.node2 = atoi(token);

			/* read value node */
			token = strtok(NULL," ");
			if( token == NULL){
				return 0;
			}
			node->source_v.value = atof(token);


			/* NO MORE TOKENS.IF FOUND RETURN ERROR */
			token = strtok(NULL," \n");
			if( token == NULL ){
				*type = NODE_SOURCE_V_TYPE;
				return 1;
			}
			else{
				/* tokens were found.print for debugging...*/
				printf("LINE: %s , garbage token : %s\n" , line , token);
				return 0;
			}

		}

		/*
		 * CURRENT SOURCE
		 */
		case 'i':
		case 'I':{

			/* read name */
			token = strtok(line," ");
			if( token == NULL ){
				return 0 ;
			}
			strcpy( node->source_i.name , token);

			/* read <+> node */
			token = strtok(NULL," ");
			if( token == NULL){
				return 0;
			}

      /* check for reference node (ground) */
      if( strcmp(token,"0") == 0 ){
        node->source_i.node1 = 0;
        list->has_reference = 1;
      }
      else{
        /*
         * this is not a reference node.Add string to
         * hash table
         */
        flag = ht_insert_pair(list->hashtable, token , node_count);
        if( flag == 1 ){
          /* successfull insertion */
          node->source_i.node1 = node_count;
          node_count++;    // get ready for the next node
        }
        else if( flag == 0 ){
          /* NULL pointer or out of memory */
          printf("Error at inserting pair to hash table..\n");
          //free_list(list);
          exit(1);
        }
        else if( flag == -1 ){

          int n;
          printf("Node : \"%s\" already on hash table \n",token);
          ht_get(list->hashtable,token,&n);
          node->source_i.node1 = n;
        }
      }


			//node->source_i.node1 = atoi(token);

			/* read <-> node */
			token = strtok(NULL," ");
			if( token == NULL){
				return 0;
			}

      /* check for reference node (ground) */
      if( strcmp(token,"0") == 0 ){
        node->source_i.node2 = 0;
        list->has_reference = 1;
      }
      else{
        /*
         * this is not a reference node.Add string to
         * hash table
         */
        flag = ht_insert_pair(list->hashtable, token , node_count);
        if( flag == 1 ){
          /* successfull insertion */
          node->source_i.node2 = node_count;
          node_count++;    // get ready for the next node
        }
        else if( flag == 0 ){
          /* NULL pointer or out of memory */
          printf("Error at inserting pair to hash table..\n");
          //free_list(list);
          exit(1);
        }
        else if( flag == -1 ){

          int n;
          printf("Node : \"%s\" already on hash table \n",token);
          ht_get(list->hashtable,token,&n);
          node->source_i.node2 = n;
        }
      }

			//node->source_i.node2 = atoi(token);

			/* read value node */
			token = strtok(NULL," ");
			if( token == NULL){
				return 0;
			}
			node->source_i.value = atof(token);


			/* NO MORE TOKENS.IF FOUND RETURN ERROR */
			token = strtok(NULL," \n");
			if( token == NULL ){
				*type = NODE_SOURCE_I_TYPE;
				return 1;
			}
			else{
				/* tokens were found.print for debugging...*/
				printf("LINE: %s  garbage token : %s\n" , line , token);
				return 0;
			}

		}

		/*
		 * MOSFET transistor
		 */
		case 'M':
		case 'm':{

			/* read name */
			token = strtok(line," ");
			if( token == NULL ){
				return 0 ;
			}
			strcpy( node->mosfet.name , token);

			/* read drain */
			token = strtok(NULL," ");
			if( token == NULL )
				return 0;

      /* check for reference node (ground) */
      if( strcmp(token,"0") == 0 ){
        node->mosfet.drain = 0;
        list->has_reference = 1;
      }
      else{
        /*
         * this is not a reference node.Add string to
         * hash table
         */
        flag = ht_insert_pair(list->hashtable, token , node_count);
        if( flag == 1 ){
          /* successfull insertion */
          node->mosfet.drain = node_count;
          node_count++;    // get ready for the next node
        }
        else if( flag == 0 ){
          /* NULL pointer or out of memory */
          printf("Error at inserting pair to hash table..\n");
          //free_list(list);
          exit(1);
        }
        else if( flag == -1 ){

          int n;
          printf("Node : \"%s\" already on hash table \n",token);
          ht_get(list->hashtable,token,&n);
          node->mosfet.drain = n;
        }
      }

			//node->mosfet.drain = atoi(token);

			/* read gate */
			token = strtok(NULL," ");
			if( token == NULL )
				return 0;
			
      /* check for reference node (ground) */
      if( strcmp(token,"0") == 0 ){
        node->mosfet.gate = 0;
        list->has_reference = 1;
      }
      else{
        /*
         * this is not a reference node.Add string to
         * hash table
         */
        flag = ht_insert_pair(list->hashtable, token , node_count);
        if( flag == 1 ){
          /* successfull insertion */
          node->mosfet.gate = node_count;
          node_count++;    // get ready for the next node
        }
        else if( flag == 0 ){
          /* NULL pointer or out of memory */
          printf("Error at inserting pair to hash table..\n");
          //free_list(list);
          exit(1);
        }
        else if( flag == -1 ){

          int n;
          printf("Node : \"%s\" already on hash table \n",token);
          ht_get(list->hashtable,token,&n);
          node->mosfet.gate = n;
        }
      }      


      //node->mosfet.gate = atoi(token);

			/* read source */
			token = strtok(NULL," ");
			if( token == NULL )
				return 0;

      /* check for reference node (ground) */
      if( strcmp(token,"0") == 0 ){
        node->mosfet.source = 0;
        list->has_reference = 1;
      }
      else{
        /*
         * this is not a reference node.Add string to
         * hash table
         */
        flag = ht_insert_pair(list->hashtable, token , node_count);
        if( flag == 1 ){
          /* successfull insertion */
          node->mosfet.source = node_count;
          node_count++;    // get ready for the next node
        }
        else if( flag == 0 ){
          /* NULL pointer or out of memory */
          printf("Error at inserting pair to hash table..\n");
          //free_list(list);
          exit(1);
        }
        else if( flag == -1 ){

          int n;
          printf("Node : \"%s\" already on hash table \n",token);
          ht_get(list->hashtable,token,&n);
          node->mosfet.source = n;
        }
      }

      //node->mosfet.source = atoi(token);

			/* read body */
			token = strtok(NULL," ");
			if( token == NULL )
				return 0;

      /* check for reference node (ground) */
      if( strcmp(token,"0") == 0 ){
        node->mosfet.body = 0;
        list->has_reference = 1;
      }
      else{
        /*
         * this is not a reference node.Add string to
         * hash table
         */
        flag = ht_insert_pair(list->hashtable, token , node_count);
        if( flag == 1 ){
          /* successfull insertion */
          node->mosfet.body = node_count;
          node_count++;    // get ready for the next node
        }
        else if( flag == 0 ){
          /* NULL pointer or out of memory */
          printf("Error at inserting pair to hash table..\n");
          //free_list(list);
          exit(1);
        }
        else if( flag == -1 ){

          int n;
          printf("Node : \"%s\" already on hash table \n",token);
          ht_get(list->hashtable,token,&n);
          node->mosfet.body = n;
        }
      }

      //node->mosfet.body = atoi(token);

			/* read length */
			token = strtok(NULL," ");
			if( token == NULL )
				return 0;
			node->mosfet.l = atof(token);

			/* read width */
			token = strtok(NULL," ");
			if( token == NULL )
				return 0;
			node->mosfet.w = atof(token);

			/* NO MORE TOKENS.IF FOUND RETURN ERROR */
			token = strtok(NULL," \n");
			if( token == NULL ){
				*type = NODE_MOSFET_TYPE;
				return 1;
			}
			else{
				/* tokens were found.print for debugging...*/
				printf("LINE: %s  garbage token : %s\n" , line , token);
				return 0;
			}
		}

		/*
		 * Bipolar junction transistor
		 */
		case 'Q':
		case 'q':{
			/* read name */
			token = strtok(line," ");
			if( token == NULL ){
				return 0 ;
			}
			strcpy( node->bjt.name , token);

			/* read collector */
			token = strtok(NULL," ");
			if( token == NULL )
				return 0;

      /* check for reference node (ground) */
      if( strcmp(token,"0") == 0 ){
        node->bjt.collector = 0;
        list->has_reference = 1;
      }
      else{
        /*
         * this is not a reference node.Add string to
         * hash table
         */
        flag = ht_insert_pair(list->hashtable, token , node_count);
        if( flag == 1 ){
          /* successfull insertion */
          node->bjt.collector = node_count;
          node_count++;    // get ready for the next node
        }
        else if( flag == 0 ){
          /* NULL pointer or out of memory */
          printf("Error at inserting pair to hash table..\n");
          //free_list(list);
          exit(1);
        }
        else if( flag == -1 ){

          int n;
          printf("Node : \"%s\" already on hash table \n",token);
          ht_get(list->hashtable,token,&n);
          node->bjt.collector = n;
        }
      }

      //node->bjt.collector = atoi(token);

			/* read base */
			token = strtok(NULL," ");
			if( token == NULL )
				return 0;

      /* check for reference node (ground) */
      if( strcmp(token,"0") == 0 ){
        node->bjt.base = 0;
        list->has_reference = 1;
      }
      else{
        /*
         * this is not a reference node.Add string to
         * hash table
         */
        flag = ht_insert_pair(list->hashtable, token , node_count);
        if( flag == 1 ){
          /* successfull insertion */
          node->bjt.base = node_count;
          node_count++;    // get ready for the next node
        }
        else if( flag == 0 ){
          /* NULL pointer or out of memory */
          printf("Error at inserting pair to hash table..\n");
          //free_list(list);
          exit(1);
        }
        else if( flag == -1 ){

          int n;
          printf("Node : \"%s\" already on hash table \n",token);
          ht_get(list->hashtable,token,&n);
          node->bjt.base = n;
        }
      }

      //node->bjt.base  = atoi(token);

			/* read emitter */
			token = strtok(NULL," ");
			if( token == NULL )
				return 0;

      /* check for reference node (ground) */
      if( strcmp(token,"0") == 0 ){
        node->bjt.emitter = 0;
        list->has_reference = 1;
      }
      else{
        /*
         * this is not a reference node.Add string to
         * hash table
         */
        flag = ht_insert_pair(list->hashtable, token , node_count);
        if( flag == 1 ){
          /* successfull insertion */
          node->bjt.emitter = node_count;
          node_count++;    // get ready for the next node
        }
        else if( flag == 0 ){
          /* NULL pointer or out of memory */
          printf("Error at inserting pair to hash table..\n");
          //free_list(list);
          exit(1);
        }
        else if( flag == -1 ){

          int n;
          printf("Node : \"%s\" already on hash table \n",token);
          ht_get(list->hashtable,token,&n);
          node->bjt.emitter = n;
        }
      }			


      //node->bjt.emitter = atoi(token);

			/* more will be added later */
			//
			//			HERE
			//


			/* NO MORE TOKENS.IF FOUND RETURN ERROR */
			token = strtok(NULL," \n");
			if( token == NULL ){
				*type = NODE_BJT_TYPE;
				return 1;
			}
			else{
				/* tokens were found.print for debugging...*/
				printf("LINE: %s  garbage token : %s\n" , line , token);
				return 0;
			}

		}

		/*
		 * DIODE
		 */

	}



	return 0;
}