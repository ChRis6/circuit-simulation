#include "linear_helper.h"
#include <math.h>

double lh_norm( gsl_vector* v){
	double res = gsl_blas_dnrm2(v)
	return res;
}

gsl_vector* lh_diag_mul(gsl_vector** v , gsl_matrix* A){
	return NULL;
}


gsl_vector* lh_matrix_vector_mul( gsl_vector** v , gsl_matrix* A){

	return NULL;
}


gsl_vector* lh_scalar_vector_mul(double s , gsl_vector** v){
	return NULL;
}


double lh_dot_product(gsl_vector* v1 , gsl_vector* v2){

	return 0;
}