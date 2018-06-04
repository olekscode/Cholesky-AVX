#include "chol.h"

void cholesky_banachiewicz(double** A, double** L, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < (i + 1); ++j) {
            double sum = 0;

            for (int k = 0; k < j; ++k) {
                sum += L[i][k] * L[j][k];
            }

            if (i == j) {
                L[i][j] = sqrt(A[i][i] - sum);
            }
            else {
                L[i][j] = (1.0 / L[j][j] * (A[i][j] - sum));
            }
        }
    }
}

void cholesky_banachiewicz_alt(double** A, double** L, int n) {
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

void cholesky_banachiewicz_super_avx(double** A, double** L, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < (i + 1); ++j) {
            double sum = dotprod(L[i], L[j], 0, j);
            L[i][j] = A[i][j] - sum;
        }

        L[i][i] = sqrt(L[i][i]);
        
        for (int j = 0; j < i; ++j) {
            L[i][j] /= L[j][j];
        }
    }
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