#include "circuit_sim_sparse.h"


int sparse_LU_decomp(sparce_matrix* matrix, css* S, csn* N ){

	if( !matrix)
		return 0;

	S=cs_sqr(2,matrix,0);
	N=cs_lu(C,matrix,1);
	
	if(!S || !N)
		return 0;

	cs_spfree(C);
}

int sparce_solve_LU(css* S, csn* N, sparse_vector* b, sparse_vector* x, int n){
	if(!S || !N || !b)
		return 0;

	if(!cs_ipvec(N->pinv,b,x,n)){
		
		return 0;
	}
	if(!cs_lsolve(N->L,x)){
		return 0;
	}
	if(!usolve(N->U,x)){
		return 0;
	}
	if(!cs_ipvec(S->q,x,b,n)){
		return 0;
	}

	cs_sfree(S);
	cs_nfree(N);
	
}





