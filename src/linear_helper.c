#include "linear_helper.h"
#include <gsl/gsl_blas.h>
#include <math.h>

double lh_norm( gsl_vector* v){
	double res = gsl_blas_dnrm2(v);
	return res;
}

gsl_vector* lh_diag_mul(gsl_vector* v , gsl_vector* A){
	
	int i;
	double temp1,temp2;

	for( i = 0 ; i < A->size ; i++){
		temp1 = gsl_vector_get(v , i);
		temp2 = gsl_vector_get(A , i);

		gsl_vector_set(v , i , temp1 * temp2);
	}

	return v;
}


gsl_vector* lh_matrix_vector_mul( gsl_vector* v , gsl_matrix* A){

	return NULL;
}


void lh_scalar_vector_mul(double s , gsl_vector* v){
	gsl_blas_dscal(s , v);
}


double lh_dot_product(gsl_vector* v1 , gsl_vector* v2){

	double dot;
	gsl_blas_ddot( v1 , v2 , &dot);

	return dot;
}