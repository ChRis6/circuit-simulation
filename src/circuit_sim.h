#ifndef CIRCUIT_SIM_H
#define CIRCUIT_SIM_H

#include <gsl/gsl_matrix.h>
#include "node_list.h"

/*
 * Create the matrix and vector for the circuit elements
 */
int create_mna(LIST *list , gsl_matrix **matrix , gsl_vector** vector);

#endif