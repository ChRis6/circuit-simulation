#include "decomposition.h"

int check_decomp_error(int error_code);

/* This is the basic decomposition function that will do a gauss or a cholesky decomposition
 * @param *A : 	A is the matrix that will be decomposed
 * @param *p : 		Gauss decomposition 	-> 	the permutation matrix
 * 					Cholesky decomposition 	->	NULL
 * @param *signum :	Gauss decomposition		-> 	the sign of permutation
 * 					Cholesky decomposition 	-> 	NULL
 * @param d_choice:	Gauss decomposition 	->	GAUSS_DECOMPOSITION
 * 					Cholesky decomposition	->	CHOLESKY DECOMPOSITION*/
int decomposition(gsl_matrix * A, gsl_permutation * p, int * signum,int decomposition_choice)
{
	if (decomposition_choice == GAUSS_DECOMPOSITION)
	{
		/*Place gauss decomposition code here */
		return 1;
	}else if(decomposition_choice == CHOLESKY_DECOMPOSITION)
	{
		/* We are setting of the handler just to do a proper checking and
		 * handle the error by our own
		 */
		gsl_set_error_handler_off();
		return check_decomp_error(gsl_linalg_cholesky_decomp(A));
		
	}
	return 0;
}



int cholesky_decomposition(gsl_matrix * A);



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