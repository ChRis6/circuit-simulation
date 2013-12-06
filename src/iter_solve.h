#ifndef ITER_SOLVE_H
#define ITER_SOLVE_H

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>


/* Iterative methods */


/*
 * Set max iterations, error tolerance
 */
void iter_set_options( int iterations , double itol );

/*
 * Solve using the cg method
 * Returns NULL on error
 */
gsl_vector* iter_solve_cg(gsl_matrix* A , gsl_vector* b , gsl_vector* x0 );


/*
 * Solve using the bicg method
 * Returns NULL on error
 */
gsl_vector* iter_solve_bicg(gsl_matrix* A , gsl_vector* b , gsl_vector* x0 );


#endif