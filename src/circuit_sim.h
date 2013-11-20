#ifndef CIRCUIT_SIM_H
#define CIRCUIT_SIM_H

#include <gsl/gsl_matrix.h>
#include "node_list.h"

/*
 * Create the matrix and vector for the circuit elements
 */
int create_mna(LIST *list , gsl_matrix **matrix , gsl_vector** vector);

/* Change the dc_sweep node in an incremental way. The value that will
 * be added is the increment that is given by the user.
 * @param vector: The vector that contains the V or I dc source
 * @param list 	: The list that contains the information we need 
 * 				 	for the dc sweep to happen
 */
double dc_sweep_increment(gsl_vector *vector,DC_SWEEP_T list);


#endif