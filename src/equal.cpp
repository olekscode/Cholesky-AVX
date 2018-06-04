#include "equal.h"

bool equal_arrays(double* a, double* b, int n, double tol) {
    for (int i = 0; i < n; ++i) {
        if (fabs(a[i] - b[i]) >= tol) {
            return false;
        }
    }
    return true;
}

bool equal_matrices(double** A, double** B, int nrow, int ncol, double tol) {
    for (int i = 0; i < nrow; ++i) {
        for (int j = 0; j < ncol; ++j) {
            if (fabs(A[i][j] - B[i][j]) >= tol) {
                return false;
            }
        }
    }
    return true;
}