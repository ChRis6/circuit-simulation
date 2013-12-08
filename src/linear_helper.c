#include "linear_helper.h"
#include <gsl/gsl_blas.h>
#include <math.h>



double lh_norm( gsl_vector* v){
	double res = gsl_blas_dnrm2(v);
	return res;
}

void lh_diag_mul(gsl_vector* res, gsl_vector* v , gsl_vector* A){
	
	int i;
	double temp1,temp2;

	for( i = 0 ; i < A->size ; i++){
		temp1 = gsl_vector_get(v , i);
		temp2 = gsl_vector_get(A , i);

		gsl_vector_set(res , i , temp1 * temp2);
	}
}

void lh_matrix_vector_mul( gsl_vector* x, gsl_matrix* A,gsl_vector* y,int transp){
	
	if (transp == TRANSP)
		gsl_blas_dgemv(CblasTrans,1,A,x,0,y);
	else if (transp == NON_TRANSP)
		gsl_blas_dgemv(CblasNoTrans,1,A,x,0,y);
}

void lh_matrix_vector_mul_and_sum( gsl_vector* x, gsl_matrix* A,gsl_vector* y,int transp,double alpha,double beta){
	
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


gsl_vector* lh_get_inv_diag(gsl_matrix* m){
	gsl_vector* res;
	res = gsl_vector_calloc(m->size1);
	if( !res )
		return NULL;

	int i;
	for( i = 0 ; i < m->size1; i++){
		gsl_vector_set(res , i ,  1 / (double) gsl_matrix_get(m,i,i));
	}

	return res;
}

void lh_scalar_vector_mul(gsl_vector* res, double s , gsl_vector* v){

	int i;
	for ( i = 0 ; i < v->size; i++){
		double temp;
		temp = gsl_vector_get(v , i);
		gsl_vector_set(res , i , s * temp);
	}
}