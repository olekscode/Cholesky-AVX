#ifndef DOTPROD_H
#define DOTPROD_H

#include <pthread.h>
#include <immintrin.h>

#include "error.h"

double dotprod(double* a, double* b, int begin, int end);
double dotprod_par(double* a, double* b, int begin, int end);
double dotprod_avx(double* a, double* b, int begin, int end);
double dotprod_avx_par(double* a, double* b, int begin, int end);

#endif // DOTPROD_H