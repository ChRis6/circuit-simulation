/*Small code for reading the netlist file line by line,
 extracting string tokens from each line,
 and recognizing the type of element described on each line.
 
 Outputs each string token on the results.txt file.
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

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
int main (int argc, char **argv)
{
	
  FILE *output_file;
  int netlist_integrity=0;  
  if(argc<3)
  {
   printf("No file name");
  }
  else
  {
    FILE *input_file=fopen(argv[1], "r");
    FILE *output_file=fopen(argv[2], "w");
   
    netlist_integrity=check_netlist_file(input_file,output_file);
	
    // remember to close all files when we are done
    fclose(input_file);
    fclose(output_file);
  }
}
*/

