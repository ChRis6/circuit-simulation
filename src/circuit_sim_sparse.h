#ifndef CIRCUIT_SIM_SPARSE_H
#define CIRCUIT_SIM_SPARSE_H

#include "csparse.h"

typedef sparse_matrix cs;
typedef sparse_vector double;

/*
 * creates a sparse mna matrix  and rhs b vector
 * Returns 0 when failed
 */
int create_mna_sparse(LIST *list, sparse_matrix**  matrix , sparse_vector* b );

/*
 * dc_sweep for sparse matrices
 */

void dc_sweep_sparse();



#endif