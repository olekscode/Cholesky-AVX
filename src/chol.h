#ifndef CHOL_H
#define CHOL_H

#include <math.h>    // sqrt

#include "dotprod.h" // dotprod, dotprod_avx

void cholesky_banachiewicz(double** A, double** L, int n);
void cholesky_banachiewicz_alt(double** A, double** L, int n);
void cholesky_banachiewicz_avx(double** A, double** L, int n);
void cholesky_banachiewicz_super_avx(double** A, double** L, int n);
void cholesky_crout_contiguous(double* A, double* L, int n);

void subtr_avx(double* a, double* b, double* c, int n);

#endif // CHOL_H