#ifndef LINEAR_HELPER_H
#define LINEAR_HELPER_H

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

double lh_norm( gsl_vector* v);

/*
 * Diagonal matrix-vector multiplication
 * Writes the result to v
 */
gsl_vector* lh_diag_mul(gsl_vector** v , gsl_vector* A);

/*
 * Returns a vector of the inversed diagonal elements of matrix m
 */
gsl_vector* lh_get_inv_diag(gsl_matrix* m);
/*
 * matrix-vector multiplication
 * Writes result to v
 */
gsl_vector* lh_matrix_vector_mul( gsl_vector** v , gsl_matrix* A);

/*
 * Vector - scalar multiplication
 * writes result to v
 */
void lh_scalar_vector_mul(double s , gsl_vector* v);

/*
 * dot product
 */
double lh_dot_product(gsl_vector* v1 , gsl_vector* v2);
#endif