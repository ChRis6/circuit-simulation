#include "iter_solve.h"
#include "linear_helper.h"

#include <math.h>

#define ABS(value)  ( (value) >=0 ? (value) : -(value) )
#define EPS 1e-14

static int iter = 10;
static double tolerance = 1e-3;


void iter_set_options( int iterations , double itol ){
	iter = iterations;
	tolerance = itol;
}

static void print_matrix_gsl(gsl_matrix* A)
{
	int i,j;

	printf("Going to print gsl_matrix \n");
	for(i = 0; i < A->size1; i++)
	{
		for(j = 0; j < A->size2; j++)
		{
			printf("\t%.4f\t",gsl_matrix_get(A,i,j));
		}
		printf("\n");
	}
}

static void print_vector_gsl(gsl_vector* A)
{
	int i;

	printf("Going to print gsl_vector \n");
	for(i = 0; i < A->size; i++)
	{
		printf("\t%.4f\n",gsl_vector_get(A,i));

	}
}

gsl_vector* iter_solve_cg(gsl_matrix* A , gsl_vector* b , gsl_vector* x0 ){

	int iteration = 0 ;

	print_vector_gsl(b);
	print_matrix_gsl(A);


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
	//print_vector_gsl(M);

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
		/*if(iteration == 2)
		{
			print_vector_gsl(p);
			printf("\n");
			print_vector_gsl(q);
			printf("\n");		
		}
		printf("\n");
		/* q = Ap */
		lh_matrix_vector_mul( p,A,q,NON_TRANSP);
		/*if(iteration == 2)
		{
			print_vector_gsl(p);
			printf("\n");
			print_vector_gsl(q);
		}*/
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
		lh_scalar_vector_mul( temp_v , alpha , q); // temp_v = alpha* q
		
		gsl_vector_sub(r,temp_v);				   // r = r - temp_v
		
		gsl_vector_free(temp_v);
	}


	/* clean up */
	gsl_vector_free(r);
	gsl_vector_free(z);
	gsl_vector_free(p);
	gsl_vector_free(M);	
	gsl_vector_free(q);

	/* result is written in x0 */
	print_vector_gsl(x0);
	return x0;
}

gsl_vector* iter_solve_bicg(gsl_matrix* A , gsl_vector* b , gsl_vector* x0 ){
	printf("mpike \n");
	print_vector_gsl(b);
	print_matrix_gsl(A);

	int iteration=0;
	double normb;
	double alpha,beta,rho,rho1,omega;

	gsl_vector *r,*r_t;
	gsl_vector *M;				// M_t is M itself.
	gsl_vector *p,*p_t;
	gsl_vector *z,*z_t;
	gsl_vector *q,*q_t;
	gsl_vector *temp;

	r = gsl_vector_calloc(b->size);
	r_t = gsl_vector_calloc(b->size);
	if( !r || !r_t ){
		return NULL;
	} 

	M = gsl_vector_calloc(b->size);
	M = lh_get_inv_diag(A);				// init m
	if( !M){
		gsl_vector_free(r);
		gsl_vector_free(r_t);

		return NULL;
	} 

	p = gsl_vector_calloc(b->size);
	p_t = gsl_vector_calloc(b->size);
	if( !p || !p_t ){
		gsl_vector_free(r);
		gsl_vector_free(r_t);
		gsl_vector_free(M);

		return NULL;
	} 

	z = gsl_vector_calloc(b->size);
	z_t = gsl_vector_calloc(b->size);
	if( !z || !z_t ){
		gsl_vector_free(r);
		gsl_vector_free(r_t);
		gsl_vector_free(M);
		gsl_vector_free(p);
		gsl_vector_free(p_t);

		return NULL;
	} 

	q = gsl_vector_calloc(b->size);
	q_t = gsl_vector_calloc(b->size);
	if( !q || !q_t ){
		gsl_vector_free(r);
		gsl_vector_free(r_t);
		gsl_vector_free(M);
		gsl_vector_free(p);
		gsl_vector_free(p_t);
		gsl_vector_free(z);
		gsl_vector_free(z_t);

		return NULL;
	}

	temp = gsl_vector_calloc(b->size);
	if( !temp ){
		gsl_vector_free(r);
		gsl_vector_free(r_t);
		gsl_vector_free(M);
		gsl_vector_free(p);
		gsl_vector_free(p_t);
		gsl_vector_free(z);
		gsl_vector_free(z_t);
		gsl_vector_free(q);
		gsl_vector_free(q_t);

		return NULL;
	}


	gsl_vector_memcpy(temp,b);
	
	/* b1 = b - Ax; */

	lh_matrix_vector_mul_and_sum( x0,A,temp,NON_TRANSP,-1.0,1.0);
	
	// r = temp & r_t = r
	gsl_vector_memcpy(r , temp);
	gsl_vector_memcpy(r_t,r);
	//gsl_vector_free(b1);

	normb = lh_norm(b);
	if(normb < EPS)
		normb =1;

	while( iteration < iter && ( (lh_norm(r) / normb) > tolerance )){

		iteration++;

		lh_diag_mul(z,r,M);			// Solve M * z = r
		lh_diag_mul(z_t,r_t,M);		// Solve M * z_t = r_t
	
		rho = lh_dot_product( r_t , z);

		if(ABS(rho) < EPS){
			printf("Algorith failed\n");
			exit(0);
		}


		if(iteration == 1){
			gsl_vector_memcpy(p , z); 	// p = z 
			gsl_vector_memcpy(p_t , z_t); 	// p_t = z_t 
		}
		else{
			beta = rho/rho1;

			lh_scalar_vector_mul(p, beta,p); //  p = beta* p
			gsl_vector_add( p , z);			 //  p = z + p

			lh_scalar_vector_mul(p_t, beta,p_t); //  p_t = beta * p_t
			gsl_vector_add( p_t , z_t);			 //  p_t = z_t + p_t

		}

		rho1 = rho;
		lh_matrix_vector_mul( p,A,q,NON_TRANSP);
		lh_matrix_vector_mul( p_t,A,q_t,TRANSP);

		omega = lh_dot_product(p_t , q);
		if( ABS(omega) < EPS ){
			printf("Algorith failed\n");
			exit(0);
		}

		alpha = rho/omega;

		/* x = x + alpha*q; */

		lh_scalar_vector_mul(temp , alpha , p); // temp = alha * p
		gsl_vector_add( x0 , temp);			  // x = x + temp


		/* 
		r = r -alpha*q; 
		r_t = r_t - alpha*q_t;
		 */

		lh_scalar_vector_mul( temp , alpha , q); // temp = alpha* q
		gsl_vector_sub(r,temp);				   // r = r - temp

		lh_scalar_vector_mul( temp , alpha , q_t); // temp = alpha* q_t
		gsl_vector_sub(r_t,temp);				   // r_t = r_t - temp_v
	}


	gsl_vector_free(r);
	gsl_vector_free(r_t);
	gsl_vector_free(M);
	gsl_vector_free(p);
	gsl_vector_free(p_t);
	gsl_vector_free(z);
	gsl_vector_free(z_t);
	gsl_vector_free(q);
	gsl_vector_free(q_t);
	gsl_vector_free(temp);


	print_vector_gsl(x0);
	return x0;
}

