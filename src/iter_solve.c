#include "iter_solve.h"
#include "linear_helper.h"

#include "math.h"

static int iter = 10;
static double tolerance = 1e-3;
static double eps = 1e-4;


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

	gsl_vector* q;
	q = gsl_vector_calloc(b->size);
	if( !q )
		return NULL;

	gsl_vector* M = lh_get_inv_diag(A);
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
	lh_matrix_vector_mul_and_sum( x0,A,b1,NON_TRANSP,-1.0,1.0); 
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
		lh_matrix_vector_mul( p,A,q,NON_TRANSP);
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

	/* result written in x0 */
	return x0;
}

gsl_vector* iter_solve_bicg(gsl_matrix* A , gsl_vector* b , gsl_vector* x0 ){
	int i = 0;
	double beta = 0;
	double rho = 0;
	double norm_r = 0;
	double norm_b = 0;
	double omega = 0;
	gsl_vector* r;
	gsl_vector* r_t;
	gsl_vector* z;
	gsl_vector* temp_z;
	gsl_vector* z_t;
	gsl_vector* temp_z_t;
	gsl_vector* m;
	gsl_vector* p;
	gsl_vector* p_t;
	gsl_vector* q;
	gsl_vector* q_t;

	r = gsl_vector_alloc(A->size1);
	r_t = gsl_vector_alloc(A->size1);
	z = gsl_vector_alloc(A->size1);
	temp_z = gsl_vector_alloc(A->size1);
	temp_z_t = gsl_vector_alloc(A->size1);
	z_t = gsl_vector_alloc(A->size1);
	p = gsl_vector_alloc(A->size1);
	p_t = gsl_vector_alloc(A->size1);

	m = lh_get_inv_diag(A);
	lh_matrix_vector_mul_and_sum(x0,A,b,NON_TRANSP,-1,1);
	gsl_vector_memcpy(r,b);
	gsl_vector_memcpy(r_t,b);

	norm_r = lh_norm(r);
	norm_b = lh_norm(b);

	while((norm_r / norm_b) > tolerance && i < iter)
	{
		iter++;
		lh_diag_mul(z , m); 	/* Preconditioner solve*/
		lh_diag_mul(z_t , m);	/*Transpose prec-solve */

		rho = lh_dot_product(z,r_t);

		if(abs(rho) < eps) 		/* Algorithm failure */
		{
			perror("Algorithm failed in iter_solve_bicg.")
			exit(0);
		}

		if (iter == 1)
		{
			gsl_vector_memcpy(p,z);
			gsl_vector_memcpy(p_t,z_t);
		}
		else
		{
			beta = rho / rho_1;
			
			gsl_vector_memcpy(temp_z,z);
			gsl_vector_memcpy(temp_z_t, z_t);
			
			gsl_vector_add (temp_z,lh_scalar_vector_mul(beta,p));
			gsl_vector_add(temp_z_t,lh_scalar_vector_mul(beta,p_t));
			
			gsl_vector_memcpy(p,temp_z);
			gsl_vector_memcpy(p_t,temp_z);
		}
		rho_1 = rho;

		lh_matrix_vector_mul( p, A, q ,NON_TRANSP); /* q = Ap */
		lh_matrix_vector_mul( p_t, A, q_t ,TRANSP); /* q~ = transposed(A)p~*/

		omega = lh_dot_product(p_t,q);

		if(abs(omega) < eps)
		{
			perror("Algorithm failed in iter_solve_bicg.")
			exit(0);
		}
		alpha = rho / omega;

	}


	return NULL;
}