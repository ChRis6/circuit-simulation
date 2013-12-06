#include "linear_helper.h"
#include <gsl/gsl_blas.h>
#include <math.h>

#define TRANSP 1
#define NON_TRANSP 2

double lh_norm( gsl_vector* v){
	double res = gsl_blas_dnrm2(v);
	return res;
}

gsl_vector* lh_diag_mul(gsl_vector** v , gsl_vector* A){
	return NULL;
}


void lh_matrix_vector_mul( gsl_vector* v , gsl_matrix* A,gsl_vector* rhs,int transp){
	
	if (transp == TRANSP)
		gsl_blas_dgemv(CblasTrans,1,A,v,0,rhs);
	else if (transp == NON_TRANSP)
		gsl_blas_dgemv(CblasNoTrans,1,A,v,0,rhs);
}


void lh_scalar_vector_mul(double s , gsl_vector* v){
	gsl_blas_dscal(s , v);
}


double lh_dot_product(gsl_vector* v1 , gsl_vector* v2){

	double dot;
	gsl_blas_ddot( v1 , v2 , &dot);

	return dot;
}