#include "sparse_interface.h"
#include "iter_solve_sparse.h"

#include <math.h>

#define ABS(value)  ( (value) >=0 ? (value) : -(value) )

static int iter = 10;
static double tolerance = 1e-3;

gsl_vector* sparse_solve_cg(sparse_matrix* A , gsl_vector* b , gsl_vector* x0 ){

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

	gsl_vector* q;
	q = gsl_vector_calloc(b->size);
	if( !q )
		return NULL;

	gsl_vector* M = lh_get_inv_diag_sparse(A);
	if( !M ){
		gsl_vector_free(r);
		gsl_vector_free(b1);
		gsl_vector_free(z);
		gsl_vector_free(p);

		gsl_vector_free(q);
		return NULL;
	}


	gsl_vector_memcpy(b1,b);

	/* r = b - Ax */
	lh_matrix_vector_mul_and_sum_sparse( x0,A,b1,NON_TRANSP,-1.0,1.0);
	gsl_vector_memcpy(r , b1);
	gsl_vector_free(b1);

	double rho,rho1;
	double beta;
	double alpha;

	while ( iteration < iter && ( (lh_norm(r) / lh_norm(b)) > tolerance ) ){

		iteration++;

		lh_diag_mul(z,r,M);	// Solve Mz = r
		rho = lh_dot_product( r , z);

		if( iteration == 1 ){
			gsl_vector_memcpy(p,z); 	// p = z
		}
		else{
			beta = rho / rho1;

			//gsl_vector* temp_v = gsl_vector_calloc(p->size);

			/* p = z + beta*p */
			lh_scalar_vector_mul(p, beta,p); //  p = beta* p
			gsl_vector_add( p , z);			 //  p = z + p
		}

		rho1 = rho;

		/* q = Ap */
		lh_matrix_vector_mul_sparse( p,A,q,NON_TRANSP);
		alpha = rho / lh_dot_product( p , q);

		/* x = x + alpha * p */
		gsl_vector* temp_v = gsl_vector_calloc(p->size);
		if( !temp_v ){
			gsl_vector_free(r);
			gsl_vector_free(b1);
			gsl_vector_free(z);
			gsl_vector_free(p);

			gsl_vector_free(q);
			return NULL;
		}
		lh_scalar_vector_mul(temp_v , alpha , p); // temp_v = alha * p
		gsl_vector_add( x0 , temp_v);			  // x = x + temp_v

		/* r = r - alpha * q */
		//gsl_vector_memcpy(temp_v , q);
		lh_scalar_vector_mul( temp_v , alpha , q); // temp_v = alpha* p
		gsl_vector_sub(r,temp_v);				   // r = r - temp_v
		gsl_vector_free(temp_v);
	}


	/* clean up */
	gsl_vector_free(r);
	gsl_vector_free(z);
	gsl_vector_free(p);
	gsl_vector_free(M);
	gsl_vector_free(q);

	/* result written in x0 */
	return x0;
}

