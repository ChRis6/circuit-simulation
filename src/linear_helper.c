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

void lh_matrix_vector_mul( gsl_vector* x, gsl_matrix* A,gsl_vector* y,int transp){
	
	if (transp == TRANSP)
		gsl_blas_dgemv(CblasTrans,1,A,x,0,y);
	else if (transp == NON_TRANSP)
		gsl_blas_dgemv(CblasNoTrans,1,A,x,0,y);
}

void lh_matrix_vector_mul_and_sum( gsl_vector* x, gsl_matrix* A,gsl_vector* y,int transp,double alpha,double,beta){
	
	if (transp == TRANSP)
		gsl_blas_dgemv(CblasTrans,alpha,A,x,beta,y);
	else if (transp == NON_TRANSP)
		gsl_blas_dgemv(CblasNoTrans,alpha,A,x,beta,y);
}


void lh_scalar_vector_mul(double s , gsl_vector* v){
	gsl_blas_dscal(s , v);
}


double lh_dot_product(gsl_vector* v1 , gsl_vector* v2){

	double dot;
	gsl_blas_ddot( v1 , v2 , &dot);

	return dot;
}