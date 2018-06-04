#include <iostream>
#include <stdlib.h>    // rand, srand
#include <time.h>      // time

#include "chol.h"
#include "dotprod.h"
#include "print.h"
#include "tictoc.h"

using namespace std;

void fill_rand(double* a, int n) {
    for (int i = 0; i < n; ++i) {
        a[i] = rand();
    }
}

void experiment(double (*f)(double*, double*, int, int),
                const char* name,
                double* a, double* b, int n, double& res) {
    CLOCK_T start = tic();
    for (int i = 0; i < n; ++i)
        res += f(a, b, 0, n);
    toc(start, name);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Too few arguments" << endl;
        return 1;
    }

    srand(time(NULL));

    // int n = atoi(argv[1]);

    double* a = new double[9];
    double* b = new double[9];
    double* c = new double[9];

    //fill_rand(a, n);
    //fill_rand(b, n);

    for (int i = 0; i < 9; ++i) {
        a[i] = i + 1;
        b[i] = 3 - i % 3;
    }

    subtr_avx(a, b, c, 9);

    print_head(a, 9);
    print_head(b, 9);
    print_head(c, 9);

    delete[] c;
    delete[] b;
    delete[] a;

    return 0;
}