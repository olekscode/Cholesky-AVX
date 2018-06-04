#include "chol.h"

void cholesky_banachiewicz(double** A, double** L, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < (i + 1); ++j) {
            double sum = dotprod(L[i], L[j], 0, j);

            if (i == j) {
                L[i][j] = sqrt(A[i][i] - sum);
            }
            else {
                L[i][j] = (1.0 / L[j][j] * (A[i][j] - sum));
            }
        }
    }
}

void cholesky_banachiewicz_avx(double** A, double** L, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < (i + 1); ++j) {
            double sum = dotprod_avx(L[i], L[j], 0, j);

            if (i == j) {
                L[i][j] = sqrt(A[i][i] - sum);
            }
            else {
                L[i][j] = (1.0 / L[j][j] * (A[i][j] - sum));
            }
        }
    }
}

void subtr_avx_step(double* a, double* b, double* c) {
    __m256d apd = _mm256_loadu_pd(a);
    __m256d bpd = _mm256_loadu_pd(b);
    __m256d cpd = _mm256_sub_pd(apd, bpd);
    _mm256_store_pd(c, cpd);
}

void subtr_avx(double* a, double* b, double* c, int n) {
    for (int i = 0; i + 4 <= n; i += 4) {
        subtr_avx_step(a + i, b + i, c + i);
    }

    for (int i = n % 4; i > 0; --i) {
        c[n - i] = a[n - i] - b[n - 1];
    }
}

void cholesky_banachiewicz_super_avx(double** A, double** L, int n) {
    double* sum_buff = new double[n];

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < (i + 1); ++j) {
            sum_buff[j] = dotprod_avx(L[i], L[j], 0, j);
        }

        subtr_avx(A[i], sum_buff, L[i], i + 1);

        L[i][i] = sqrt(L[i][i]);
        
        for (int j = 0; j < i; ++j) {
            L[i][j] = 1.0 / L[j][j] * L[i][j];
        }
    }

    delete[] sum_buff;
}

void cholesky_crout_contiguous(double* A, double* L, int n) {
    for (int j = 0; j < n; j++) {
        float sum = 0;

        for (int k = 0; k < j; k++) {
            sum += L[j * n + k] * L[j * n + k];
        }

        L[j * n + j] = sqrt(A[j * n + j] - sum);

        for (int i = j + 1; i < n; i++) {
            sum = 0;

            for (int k = 0; k < j; k++) {
                sum += L[i * n + k] * L[j * n + k];
            }

            L[i * n + j] = (1.0 / L[j * n + j] * (A[i * n + j] - sum));
        }
    }
}