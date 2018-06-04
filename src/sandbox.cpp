#include <iostream>
#include <stdlib.h>    // rand, srand
#include <time.h>      // time

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

    int n = atoi(argv[1]);

    double* a = new double[n];
    double* b = new double[n];

    fill_rand(a, n);
    fill_rand(b, n);

    double res = 0;

    experiment(&dotprod_par, "Dot product parallel", a, b, n, res);
    experiment(&dotprod, "Dot product", a, b, n, res);
    experiment(&dotprod_par, "Dot product parallel", a, b, n, res);
    experiment(&dotprod, "Dot product", a, b, n, res);
    experiment(&dotprod_avx, "Dot product AVX", a, b, n, res);
    experiment(&dotprod_avx_par, "Dot product AVX parallel", a, b, n, res);
    
    cout << res << endl;

    delete[] b;
    delete[] a;

    return 0;
}