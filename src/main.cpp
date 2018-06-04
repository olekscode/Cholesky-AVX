#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "chol.h"
#include "print.h"      // print_head, print_error
#include "tictoc.h"     // tic, toc
#include "equal.h"      // equal_matrices
#include "error.h"

using namespace std;

string get_fname(int n) {
    ostringstream fname;
    fname << "data/matrix" << n << ".dat";
    return fname.str();
}

void read_matrix(string fname, double** matrix, int nrow, int ncol) {
    ifstream file(fname, ios::in|ios::binary);
    double number;

    if (file.is_open()) {
        for (int i = 0; i < nrow; ++i) {
            for (int j = 0; j < ncol; ++j) {
                file.read(reinterpret_cast<char*>(&number), sizeof(double));
                matrix[i][j] = number;
            }
        }
        file.close();
    }
    else throw ErrorOpeningFile();
}

void read_matrix_contiguous(string fname, double* matrix, int nrow, int ncol) {
    ifstream file(fname, ios::in|ios::binary);
    double number;

    if (file.is_open()) {
        for (int i = 0; i < nrow; ++i) {
            for (int j = 0; j < ncol; ++j) {
                file.read(reinterpret_cast<char*>(&number), sizeof(double));
                matrix[i * ncol + j] = number;
            }
        }
        file.close(); 
    }
    else throw ErrorOpeningFile();
}

double** alloc_matrix(int nrow, int ncol) {
    cout << "Allocating the memory for matrix" << endl;
    double** matrix = new double*[nrow];
    for (int i = 0; i < nrow; ++i) {
        matrix[i] = new double[ncol];
    }
    return matrix;
}

void delete_matrix(double** matrix, int nrow) {
    for (int i = 0; i < nrow; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
    cout << "Matrix memory freed" << endl;
}

double* alloc_array(int n) {
    cout << "Allocating the memory for an array" << endl;
    double* arr = new double[n];
    return arr;
}

void delete_array(double* arr) {
    delete[] arr;
    cout << "Array memory freed" << endl;
}

void initialize_zeros(double* arr, int n) {
    for (int i = 0; i < n; ++i) {
        arr[i] = 0;
    }
}

void initialize_zeros(double** M, int nrow, int ncol) {
    for (int i = 0; i < nrow; ++i) {
        initialize_zeros(M[i], ncol);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Too few arguments" << endl;
        return 1;
    }

    int n = atoi(argv[1]);

    CLOCK_T start;

    double** A = alloc_matrix(n, n);
    // double* Aco = alloc_array(n * n);

    string fname = get_fname(n);
    start = tic();

    try {
        read_matrix(fname, A, n, n);
        // read_matrix_contiguous(fname, Aco, n, n);
    }
    catch (Error& e) {
        delete_matrix(A, n);
        // delete_array(Aco);
        print_error(e.what());
        return 1;
    }
    catch (...) {
        delete_matrix(A, n);
        // delete_array(Aco);
        print_error("Unknown exception");
        return 1;
    }

    toc(start, "Reading matrix");

    int m = 4;
    double*** Larr = new double**[m];

    for (int i = 0; i < m; ++i) {
        Larr[i] = alloc_matrix(n, n);
    }
    // double* Lco = alloc_array(n);
    // double** B = alloc_matrix(n, n);

    start = tic();
    for (int i = 0; i < m; ++i) {
        initialize_zeros(Larr[i], n, n);
    }
    toc(start, "Initializing L with zeros");

    // print_head(A, 5, 5);

    start = tic();
    cholesky_banachiewicz(A, Larr[0], n);
    toc(start, "Cholesky-Banachiewicz");

    start = tic();
    cholesky_banachiewicz_alt(A, Larr[1], n);
    toc(start, "Cholesky-Banachiewicz");

    start = tic();
    cholesky_banachiewicz_avx(A, Larr[2], n);
    toc(start, "AVX Cholesky-Banachiewicz");

    start = tic();
    cholesky_banachiewicz_super_avx(A, Larr[3], n);
    toc(start, "Super AVX Cholesky-Banachiewicz");

    double tol = 0.01;

    cout << equal_matrices(Larr[0], Larr[1], n, n, tol) << endl;
    cout << equal_matrices(Larr[1], Larr[2], n, n, tol) << endl;
    cout << equal_matrices(Larr[0], Larr[2], n, n, tol) << endl;
    cout << equal_matrices(Larr[2], Larr[3], n, n, tol) << endl;

    print_head(Larr[0], 5, 4);
    print_head(Larr[1], 5, 4);
    print_head(Larr[2], 5, 4);

    // start = tic();
    // cholesky_crout_contiguous(Aco, Lco, n);
    // toc(start, "Cholesky-Crout contiguous");

    // print_head(L, 5, 5);

    // start = tic();
    // cholmatmul(L, B, n);
    // toc(start, "Matrix multiplication");

    // print_head(B, 5, 5);

    delete_matrix(A, n);
    // delete_array(Aco);

    for (int i = 0; i < m; ++i) {
        delete_matrix(Larr[i], n);
    }
    delete[] Larr;

    //delete_array(Lco);
    // delete_matrix(B, n);
    return 0;
}
