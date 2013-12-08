#ifndef LINEAR_HELPER_H
#define LINEAR_HELPER_H

#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

#define TRANSP 1
#define NON_TRANSP 2

double lh_norm( gsl_vector* v);

/*
 * Diagonal matrix-vector multiplication
 * Writes the result to v
 */
gsl_vector* lh_diag_mul(gsl_vector* v , gsl_vector* A);

/*
 * Returns a vector of the inversed diagonal elements of matrix m
 */
gsl_vector* lh_get_inv_diag(gsl_matrix* m);

/*
	This function computes the matrix-vector product and sum y = \alpha op(A) x + \beta y,
	where op(A) = A, A^T, A^H for TransA = CblasNoTrans, CblasTrans, CblasConjTrans.
	We set 
		alpha = 1
		beta = 0
	so we have 
		y = op(A) * x
*/
void lh_matrix_vector_mul( gsl_vector* x, gsl_matrix* A,gsl_vector* y,int transp);

/*
	This function computes the matrix-vector product and sum y = \alpha op(A) x + \beta y,
	where op(A) = A, A^T, A^H for TransA = CblasNoTrans, CblasTrans, CblasConjTrans.

*/
void lh_matrix_vector_mul_and_sum( gsl_vector* x, gsl_matrix* A,gsl_vector* y,int transp,double alpha,double,beta);


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