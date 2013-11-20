#ifndef PLOT_H
#define PLOT_H

#include <gsl/gsl_vector.h>

/*
 * Return the size  of an array of vectors.
 * endv < startv
 */ 
int plot_find_size( double startv , double endv , double inc );

/*
 * Gets an array of gsl_vectors.Starting values = 0.0
 * returns : NULL at failure
 */
gsl_vector** plot_init_vector( int array_size , int vector_size );

#endif