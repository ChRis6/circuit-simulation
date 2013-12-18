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

	//temp_vec = gsl_vector_ptr(v,0);
	for( i = 0 ; i < A->size ; i++){
		temp1 = gsl_vector_get(v , i);
		temp2 = gsl_vector_get(A , i);

		gsl_vector_set(res , i , temp1 * temp2);
	}
}



void lh_matrix_vector_mul_sparse( gsl_vector* x, sparse_matrix* A,gsl_vector* y,int transp){

	int i,j,p;
	
	if (transp == TRANSP)
	{
		
		for (i=0; i < A->n; i++ )
		{
			gsl_vector_set(y,i,0.0);
			
			for(p = A->p[i]; p < A->p[i+1]; p++)
			{
				gsl_vector_set(y,i, gsl_vector_get(y,i) + A->x[p]*gsl_vector_get(x,A->i[p]));
			}
		}
	}
		
	else if (transp == NON_TRANSP)
	{
		for(j = 0; j < A->n; j++)
		{
			for(p = A->p[j]; p < A->p[j+1]; p++)
			{
				gsl_vector_set(y,A->i[p],gsl_vector_get(y,A->i[p]) + (A->x[p]) * gsl_vector_get(x,j));
			}
		}
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


void lh_matrix_vector_mul_and_sum_sparse( gsl_vector* x, sparse_matrix* A,gsl_vector* y,int transp,double alpha,double beta){
	
	int i;
	gsl_vector* y_temp;
	y_temp = gsl_vector_calloc(x->size);
	
	/*y_temp = op(A)*x */
	lh_matrix_vector_mul_sparse(x,A,y_temp,transp);
	
	for(i = 0; i < x->size; i++)
	{
		/*y_temp = alpha * op(A)*x */
		gsl_vector_set(y_temp,i,gsl_vector_get(y_temp,i)*alpha);
		/*y = beta*b */
		gsl_vector_set(y,i,gsl_vector_get(y,i)*beta);
		/*y = beta*b + y_temp*/
		gsl_vector_set(y,i,gsl_vector_get(y,i) + gsl_vector_get(y_temp,i));

	}
}

double lh_dot_product(gsl_vector* v1 , gsl_vector* v2){

	double dot = 0.0;
	gsl_blas_ddot( v1 , v2 , &dot) ;

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

/*
gsl_vector* lh_get_inv_diag_sparse(sparse_matrix* m){
	gsl_vector* res;
	res = gsl_vector_calloc(m->size1);
	if( !res )
		return NULL;

	int i;
	for( i = 0 ; i < m->size1; i++){
		gsl_vector_set(res , i ,  1 / mi,i));
	}

	return res;
}

*/

void lh_scalar_vector_mul(gsl_vector* res, double s , gsl_vector* v){

	int i;
	for ( i = 0 ; i < v->size; i++){
		double temp;
		temp = gsl_vector_get(v , i);
		gsl_vector_set(res , i , s * temp);
	}
}