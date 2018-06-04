/** Functions for testing the equality of floating point structures.
* 
* Vectors and matrices with double entries can not be compared with
* simple '=' operator because of the floating point error.
* Therefore we compare them as
*     abs(a - b) < tol
* where tol is some tolerance */

#ifndef EQUAL_H
#define EQUAL_H

#include <math.h>

/** Tests if two arrays of the same size are equal with given tolerance */
bool equal_arrays(double* a, double* b, int n, double tol);

/** Tests if two matrices of the same size are equal with given tolerance */
bool equal_matrices(double** A, double** B, int nrow, int ncol, double tol);

#endif // EQUAL_H