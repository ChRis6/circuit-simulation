#ifndef DECOMPOSITION_H_
#define DECOMPOSITION_H_

#include <gsl/gsl_linalg.h>

#define GAUSS_DECOMPOSITION 1
#define CHOLESKY_DECOMPOSITION 2
/* This is the basic decomposition function that will do a gauss or a cholesky decomposition
 * @param *A : 	A is the matrix that will be decomposed
 * @param *p : 		Gauss decomposition 	-> 	the permutation matrix
 * 					Cholesky decomposition 	->	NULL
 * @param *signum :	Gauss decomposition		-> 	the sign of permutation
 * 					Cholesky decomposition 	-> 	NULL
 * @param d_choice:	Gauss decomposition 	->	GAUSS_DECOMPOSITION
 * 					Cholesky decomposition	->	CHOLESKY DECOMPOSITION*/
int decomposition(gsl_matrix * A, gsl_permutation * p, int * signum,int decomposition_choice);

int cholesky_decomposition(gsl_matrix * A);

int gauss_decomposition(gsl_matrix *A,gsl_permutation * p, int * signum);
#endif /*ELIMINATION_H_*/
