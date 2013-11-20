#include "plot.h"

/*
 * Return the size  of an array of vectors
 */ 
int plot_find_size( double startv , double endv , double inc ){

	double iter;
	unsigned int num_elements = 0;
	for( iter = startv ; iter <= endv ; iter = iter + inc )
		num_elements++;


	return num_elements;
}

/*
 * Gets an array of gsl_vectors.Starting values = 0.0
 * returns : NULL at failure
 */
gsl_vector** plot_init_vector( int array_size  ,  int vector_size ){

	int i;
	gsl_vector **vector_array = NULL;
	if( array_size < 1 )
		return NULL;

	vector_array = ( gsl_vector**) malloc( sizeof(gsl_vector*) * array_size );
	if( !vector_array )
		return NULL;

	for( i = 0 ; i < array_size ; i++){
		
		vector_array[i] = gsl_vector_calloc( vector_size );
		if( !(vector_array[i]) ){
			printf("Out of memory...\n");
			exit(1);
		}
	}

	return vector_array;
}