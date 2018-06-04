#ifndef PRINT_H
#define PRINT_H

#include <iostream>    // cout, endl

using namespace std;

#define GREEN "\033[38;5;2m"
#define RED "\033[38;5;1m"
#define ESCAPE "\033[0m"

/** Prints the head of a matrix */
void print_head(double** matrix, int nrow, int ncol);

/** Prints the head of an array */
void print_head(double* arr, int n);

/** Prints the elapsed time */
void print_time(const char* name, float sec);

/** Prints the error message */
void print_error(const char* msg);

#endif // PRINT_H