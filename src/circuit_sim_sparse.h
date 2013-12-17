#ifndef CIRCUIT_SIM_SPARSE_H
#define CIRCUIT_SIM_SPARSE_H

#include "node_list.h"
#include "sparse_interface.h"

/*
 * creates a sparse mna matrix  and rhs b vector
 * Returns 0 when failed
 */
int create_mna_sparse(LIST *list, sparse_matrix**  matrix , sparse_vector* b );

/*
 * dc_sweep for sparse matrices
 */
void dc_sweep_sparse();


int sparse_solve_LU();


int sparse_solve_cholesky();



#endif