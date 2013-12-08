#include "iter_solve.h"
#include "linear_helper.h"

static int iter = 10;
static double tolerance = 1e-3;


void iter_set_options( int iterations , double itol ){
	iter = iterations;
	tolerance = itol;
}


gsl_vector* iter_solve_cg(gsl_matrix* A , gsl_vector* b , gsl_vector* x0 ){

	int iteration = 0 ;

	gsl_vector* r;
	r = gsl_vector_calloc(b->size);
	if( !r )
		return NULL;

	gsl_vector* b1;
	b1 = gsl_vector_calloc(b->size);
	if( !b1)
		return NULL; 

	gsl_vector* z;
	z = gsl_vector_calloc(b->size);
	if( !z)
		return NULL;

	gsl_vector* p;
	p = gsl_vector_calloc(b->size);
	if( !p )
		return NULL;

	gsl_vector* M = lh_get_inv_diag(A);
	if( !M ){
		gsl_vector_free(r);
		gsl_vector_free(b1);
		gsl_vector_free(z);
		gsl_vector_free(p);
		return NULL;
	}


	gsl_vector_memcpy(b1,b);
	
	/* r = b - Ax */
	lh_matrix_vector_mul_and_sum( x0,A,b1,NON_TRANSP,-1.0,1.0); 
	gsl_vector_memcpy(r , b1);
	gsl_vector_free(b1);

	double rho,rho1;
	double beta;

	while ( iteration < iter && ( (lh_norm(r) / lh_norm(b)) > tolerance ) ){

		iteration++;

		lh_diag_mul(z,r,M);	// Solve Mz = r
		rho = lh_dot_product( r , z);

		if( iteration == 1 ){
			gsl_vector_memcpy(p,z); 	// p = z 
		}
		else{
			beta = rho / rho1;

		}


	}


	return NULL;
}

gsl_vector* iter_solve_bicg(gsl_matrix* A , gsl_vector* b , gsl_vector* x0 ){
	return NULL;
}