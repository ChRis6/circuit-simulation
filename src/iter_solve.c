#include "iter_solve.h"

static int iter = 10;
static double tolerance = 1e-3;


void iter_set_options( int iterations , double itol ){


}


gsl_vector* iter_solve_cg(gsl_matrix* A , gsl_vector* b , gsl_vector* x0 ){

	return NULL;
}

gsl_vector* iter_solve_bicg(gsl_matrix* A , gsl_vector* b , gsl_vector* x0 ){
	return NULL;
}