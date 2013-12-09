#include "iter_solve.h"
#include "linear_helper.h"

#include "math.h"

static int iter = 20;
static double tolerance = 1e-3;
static long double eps = 1e-4;


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


	/* clean up */
	gsl_vector_free(r);
	//gsl_vector_free(b1);
	gsl_vector_free(z);
	gsl_vector_free(p);	
	gsl_vector_free(q);
	gsl_vector_free(M);

	/* result written in x0 */
	return x0;
}

gsl_vector* iter_solve_bicg(gsl_matrix* A , gsl_vector* b , gsl_vector* x0 ){

	int iteration = 0;

	gsl_vector* r;
	gsl_vector* r_t;
	gsl_vector* b1;

	gsl_vector* M;
	gsl_vector* z_t;
	gsl_vector* z;
	gsl_vector* p;
	gsl_vector* p_t;

	gsl_vector* q;
	gsl_vector* q_t;

	double rho,rho1;
	double beta;
	double alpha;
	double omega;

	r = gsl_vector_calloc(b->size);
	if( !r )
		return NULL;

	r_t = gsl_vector_calloc(b->size);
	if( !r_t ){
		gsl_vector_free(r);
		return NULL;
	}

	b1 = gsl_vector_calloc(b->size);
	if ( !b1 ){
		gsl_vector_free(r);
		gsl_vector_free(r_t);
		return NULL;
	}

	z = gsl_vector_calloc(b->size);
	if ( !z ){
		gsl_vector_free(r);
		gsl_vector_free(r_t);
		gsl_vector_free(b1);

		return NULL;
	}

	z_t = gsl_vector_calloc(b->size);
	if ( !z_t ){
		gsl_vector_free(r);
		gsl_vector_free(r_t);
		gsl_vector_free(b1);
		gsl_vector_free(z);
		return NULL;
	}	

	p_t = gsl_vector_calloc(b->size);
	if ( !p_t ){
		gsl_vector_free(r);
		gsl_vector_free(r_t);
		gsl_vector_free(b1);
		gsl_vector_free(z);
		gsl_vector_free(z_t);
		return NULL;
	}

	p = gsl_vector_calloc(b->size);
	if ( !p_t ){
		gsl_vector_free(r);
		gsl_vector_free(r_t);
		gsl_vector_free(b1);
		gsl_vector_free(z);
		gsl_vector_free(z_t);
	
		gsl_vector_free(p_t);
		return NULL;
	}

	q = gsl_vector_calloc(b->size);
	if ( !p_t ){
		gsl_vector_free(r);
		gsl_vector_free(r_t);
		gsl_vector_free(b1);
		gsl_vector_free(z);
		gsl_vector_free(z_t);
	
		gsl_vector_free(p_t);
		gsl_vector_free(p);
		return NULL;
	}

	q_t = gsl_vector_calloc(b->size);
	if ( !p_t ){
		gsl_vector_free(r);
		gsl_vector_free(r_t);
		gsl_vector_free(b1);
		gsl_vector_free(z);
		gsl_vector_free(z_t);
	
		gsl_vector_free(p_t);
		gsl_vector_free(p);
		gsl_vector_free(q);
		return NULL;
	}

	gsl_vector_memcpy(b1 , b);

	/* r = b - AX */
	lh_matrix_vector_mul_and_sum( x0,A,b1,NON_TRANSP,-1.0,1.0); 
	gsl_vector_memcpy(r,b1);
	gsl_vector_memcpy(r_t,b1);


	M = lh_get_inv_diag(A);
	if ( !M ){
		printf("Inv diag failed...\n");
		exit(1);
	}



/*
	int i;
	for( i = 0 ; i < b->size ; i++){
		printf("r = %f b = %f r_t = %f\n", gsl_vector_get(r,i) , gsl_vector_get(b,i) , gsl_vector_get(r_t,i)   );

	}
*/
	while ( (lh_norm(r) / lh_norm(b1) ) > tolerance && iteration < iter ) {

		iteration++;
		lh_diag_mul(z , r, M);
		lh_diag_mul(r_t, r_t , M);

		rho = lh_dot_product( z,r_t);
		printf("rho = %f\n",rho);
		if( abs(rho) < eps ){
			fprintf(stderr,"Algorithm failed... (1) iter = %d \n",iteration);
			exit(1);
		}

		if( iteration == 1){
			gsl_vector_memcpy(p , z);
			gsl_vector_memcpy(p_t , z_t);
		}
		else {

			beta = rho / rho1;
			gsl_vector* temp_p = gsl_vector_calloc(b->size);
			if ( !temp_p ){
				fprintf(stderr, "Error while allocating memory...\n" );
				exit(1);
			}

			/* p = z + beta * p; */

			lh_scalar_vector_mul(temp_p , beta , p);  // temp_p = beta * p
			gsl_vector_add( temp_p , z );			  // temp_p = z + temp_p 
			gsl_vector_memcpy(p , temp_p);			  // p = temp_p

			/* p_t = z_t + beta * p_t; */
			lh_scalar_vector_mul( temp_p , beta , p_t); // temp_p = beta * p_t
			gsl_vector_add(temp_p , z_t);				// temp_p = z + temp_p
			gsl_vector_memcpy(p_t , temp_p);

			gsl_vector_free( temp_p);
		}

		rho1 = rho;
		lh_matrix_vector_mul( p,A,q,NON_TRANSP);
		lh_matrix_vector_mul( p_t,A,q_t,TRANSP);

		omega = lh_dot_product( p_t , q);
		if ( abs(omega) < eps ){
			fprintf(stderr, "Algorithm failed...(2)\n" );
			exit(1);
		}


		alpha = rho / omega;

		/* x = x + alpha * p; */

		gsl_vector* temp_p = gsl_vector_calloc(b->size);
		if ( !temp_p ){
			fprintf(stderr, "Error while allocating memory...\n" );
			exit(1);
		}

		lh_scalar_vector_mul( temp_p , alpha , p); // temp_p = alpha * p
		gsl_vector_add( x0 , temp_p);				// x = x + temp_p
	
		/* r = r - alpha*q; */
		lh_scalar_vector_mul(temp_p , alpha , q);
		gsl_vector_sub(r , temp_p);

		/* r_t = r_t - alpha* q_t ; */
		lh_scalar_vector_mul(temp_p , alpha , q_t);
		gsl_vector_sub(r_t , temp_p);

		gsl_vector_free(temp_p);

	}


	return x0;
}