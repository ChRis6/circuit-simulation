#include "decomposition.h"
#include <gsl/gsl_permutation.h>

int check_decomp_error(int error_code);

/* This is the basic decomposition function that will do a gauss or a cholesky decomposition
 * @param *A : 	A is the matrix that will be decomposed
 * @param *p : 		Gauss decomposition 	-> 	the permutation matrix
 * 					Cholesky decomposition 	->	NULL
 * @param *signum :	Gauss decomposition		-> 	the sign of permutation
 * 					Cholesky decomposition 	-> 	NULL
 * @param d_choice:	Gauss decomposition 	->	GAUSS_DECOMPOSITION
 * 					Cholesky decomposition	->	CHOLESKY DECOMPOSITION*/
int decomposition(gsl_matrix * matrix, gsl_permutation ** p, int * signum,int decomposition_choice)
{
	int permutation_len;
	
	if(matrix != NULL)
		permutation_len = matrix->size1;

	if (decomposition_choice == GAUSS_DECOMPOSITION)
	{
		permutation_len = matrix->size1;
	 	*p = gsl_permutation_alloc(permutation_len);
	 	if( !p ){
	 		printf("(No memory for permutation)\n" );
	 		exit(1);
	 	}
	 	
	 	gsl_permutation_init(*p);
	 	printf("%d\n", gsl_permutation_valid(*p) );

	 	if( gsl_linalg_LU_decomp(matrix , *p , signum ) ){
	 		printf("LU returned 0\n");
	 		exit(1);
	 	}

		return 1;
	}else if(decomposition_choice == CHOLESKY_DECOMPOSITION)
	{
		permutation_len = matrix->size1;
		p = NULL;
		/* We are setting of the handler just to do a proper checking and
		 * handle the error by our own
		 */
		//gsl_set_error_handler_off();
 		printf("CHOLESKY...\n");
 		return check_decomp_error(gsl_linalg_cholesky_decomp(matrix));		
	}else
	{
		perror("Not a valid decomposition asked\n");
	}
			
	
	return 0;
}

void solve(gsl_matrix * matrix,gsl_vector * vector,gsl_vector * x,gsl_permutation* permutation,int decomposition_choice)
{
	
	//gsl_permutation_fprintf (stderr, permutation, " %u");

	if (decomposition_choice == CHOLESKY_DECOMPOSITION)
		gsl_linalg_cholesky_solve(matrix,vector,x);
	else{
		
		gsl_permute_vector(permutation,vector);
 		gsl_linalg_LU_solve(matrix , permutation , vector , x );
	}

}




int check_decomp_error(int error_code)
{
	char error_message[100];
	if(error_code == GSL_EDOM)
	{
		sprintf(error_message,"Error detected in the cholesky decomposition: Non positive 	\n");
		perror(error_message);
		return -1;
	}
	return 1;
}