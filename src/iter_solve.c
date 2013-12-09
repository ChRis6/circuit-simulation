#include "iter_solve.h"
#include "linear_helper.h"

#include "math.h"

static int iter = 10;
static double tolerance = 1e-3;
static long double eps = 1e-14;


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
	//gsl_vector_free(r);
	//gsl_vector_free(b1);
	//gsl_vector_free(z);
	//gsl_vector_free(p);
		
	//gsl_vector_free(q);

	/* result written in x0 */
	return x0;
}

gsl_vector* iter_solve_bicg(gsl_matrix* A , gsl_vector* b , gsl_vector* x0 ){
	int i = 0;
	double alpha = 0;
	double beta = 0;
	double rho = 0;
	double rho_1 = 1;
	double norm_r = 0;
	double norm_b = 0;
	double omega = 0;

	gsl_vector* r , *r_t;
	gsl_vector* z , *z_t , *temp_z , *temp_z_t;
	gsl_vector* m;
	gsl_vector* p , *p_t;
	gsl_vector* q , *q_t;

	for ( i = 0; i < b->size ; i++)
	{
		printf("%g\n",gsl_vector_get(b,i));
	}
	/* r = r~ = b - Ax */

	r = gsl_vector_alloc(b->size);
	r_t = gsl_vector_alloc(b->size);
	
	lh_matrix_vector_mul_and_sum(x0,A,b,NON_TRANSP,-1.0,1.0);
	
	r = gsl_vector_alloc(A->size1);
	r_t = gsl_vector_alloc(A->size1);
	if(r == NULL || r_t == NULL)
	{
		perror("Allocation failed... I am going to exit now");
		exit (1);
	}

	gsl_vector_memcpy(r,b);
	gsl_vector_memcpy(r_t,b);
	


	/* 	Allocate all the vectors that we need in order for the algorithm
		to be functional 
	*/
	z = gsl_vector_alloc(b->size);
	temp_z = gsl_vector_alloc(b->size);
	temp_z_t = gsl_vector_alloc(b->size);
	z_t = gsl_vector_alloc(b->size);
	if ( z == NULL || z_t == NULL || temp_z == NULL || temp_z_t == NULL)
	{
		gsl_vector_free(r);
		gsl_vector_free(r_t);

		perror("Allocation failed... I am going to exit now");
		exit (1);
	}
	p = gsl_vector_alloc(b->size);
	p_t = gsl_vector_alloc(b->size);
	if(p == NULL || p_t == NULL)
	{
		gsl_vector_free(r);
		gsl_vector_free(r_t);
		gsl_vector_free(z);
		gsl_vector_free(z_t);
		gsl_vector_free(temp_z);
		gsl_vector_free(temp_z_t);

		perror("Allocation failed... I am going to exit now");
		exit (1);
	}
 	q = gsl_vector_alloc(b->size);
	q_t = gsl_vector_alloc(b->size);

	/* Get and save the 1/diag(A) in the vector m */
	m = lh_get_inv_diag(A);

	/* get the euclidian norms of r and b vectors */
	norm_r = lh_norm(r);
	norm_b = lh_norm(b);

	while(( abs(norm_r) / abs(norm_b) ) > tolerance && i < iter)
	{

	
		i++;
		lh_diag_mul(z,r,m); 	/* 	Preconditioner solve*/
		lh_diag_mul(z_t,r_t,m);	/*	Transpose prec-solve */


		rho = lh_dot_product(z,r_t);

		printf("rho =  %lf \n",rho);
		/*if(abs(rho) < eps) 		/* Algorithm failure 
		{
			printf("rho =  %lf \n",rho);
			perror("Algorithm failed in iter_solve_bicg ---> rho");
			exit(1);
		}*/

		if (iter == 1)
		{
			gsl_vector_memcpy(p,z); 	/* p = z */
			gsl_vector_memcpy(p_t,z_t); /* p~ = z~ */
		}
		else
		{
			beta = rho / rho_1;
			
			gsl_vector_memcpy(temp_z,z); 		/* 	temp_z = z */
			gsl_vector_memcpy(temp_z_t, z_t);	/*	temp_z_t = z_t */
			
			lh_scalar_vector_mul(p,beta,p);		/* p = beta * p where beta scalar and p vector*/
			lh_scalar_vector_mul(p_t,beta,p_t);	/* p~ = beta * p~ where beta scalar and p vector*/
			gsl_vector_add (temp_z,p);			/* temp_z = temp_z + p */
			gsl_vector_add(temp_z_t,p_t);		/* temp_z~ = temp_z~ + p~ */
			
			gsl_vector_memcpy(p,temp_z);		/* p = z + beta * p */
			gsl_vector_memcpy(p_t,temp_z);		/* p~ = z~ + beta * p~ */
		}
		rho_1 = rho;

		int j;
		for (j = 0; j < p->size; j++)
		{
			printf("P: %lf\n",gsl_vector_get(p,j));
		}


		lh_matrix_vector_mul( p, A, q ,NON_TRANSP); /* q = Ap */
		lh_matrix_vector_mul( p_t, A, q_t ,TRANSP); /* q~ = transposed(A)p~*/
		for (j = 0; j < p->size; j++)
		{
			printf("Q: %lf\n",gsl_vector_get(q_t,j));
		}
		
		omega = lh_dot_product(p_t,q);
		printf("omega =  %lf \n",omega);

		/*if(abs(omega) < eps)
		{
			perror("Algorithm failed in iter_solve_bicg ----> omega");
			exit(1);
		}*/
		alpha = rho / omega;
		lh_scalar_vector_mul(p,alpha,p);
		gsl_vector_add (x0,p);
		lh_scalar_vector_mul(q,alpha,q);
		gsl_vector_sub (r,q);
		lh_scalar_vector_mul(q_t,alpha,q_t);
		gsl_vector_sub (r_t,q_t);
	}


	return x0;
}