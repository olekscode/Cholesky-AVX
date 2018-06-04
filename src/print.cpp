#include "print.h"

void print_head(double** matrix, int nrow, int ncol) {
    for (int i = 0; i < nrow; ++i) {
        for (int j = 0; j < ncol; ++j) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void print_head(double* arr, int n) {
    cout << "[";

    for (int i = 0; i < n; ++i) {
        cout << arr[i] << ", ";
    }
    cout << "...]" << endl;
}

void print_time(const char* name, float sec) {
    cout << GREEN << name << ": " << sec << " s" << ESCAPE << endl;
}

void print_error(const char* msg) {
    cerr << RED << "Error: " << msg << endl;
}