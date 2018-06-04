#include "tictoc.h"

CLOCK_T tic() {
    return chrono::high_resolution_clock::now();
}

void toc(CLOCK_T start, const char* name) {
    CLOCK_T end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start; 
    print_time(name, elapsed.count());
}