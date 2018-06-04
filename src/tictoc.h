#ifndef TICTOC_H
#define TICTOC_H

#include <iostream>    // cout, endl
#include <chrono>

#include "print.h"

using namespace std;

#define CLOCK_T chrono::time_point<chrono::high_resolution_clock>

CLOCK_T tic();
double toc(CLOCK_T start, const char* name);

#endif // TICTOC_H