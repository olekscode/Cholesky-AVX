#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "chol.h"
#include "tictoc.h"

using namespace std;

#define OUTPUT_FILE "data/experiment_results.csv"

typedef void(*method_t)(double**,double**,int);

typedef struct {
    const char* method_name;
    int input_size;
    double elapsed_time;
} ExperimentResult;

void save_results(ExperimentResult result) {
    ofstream file;
    file.open(OUTPUT_FILE, ios::out|ios::app);
    file << result.method_name << ","
         << result.input_size << ","
         << result.elapsed_time << "\n";
    file.close();
}

// TODO: Move this to separate file
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

double** alloc_matrix(int nrow, int ncol) {
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
}

ExperimentResult experiment(method_t method, const char* method_name, int input_size) {
    double** A = alloc_matrix(input_size, input_size);
    double** L = alloc_matrix(input_size, input_size);

    string fname = get_fname(input_size);
    read_matrix(fname, A, input_size, input_size);

    ExperimentResult result;
    result.method_name = method_name;
    result.input_size = input_size;

    CLOCK_T start = tic();

    method(A, L, input_size);

    result.elapsed_time = toc(start, method_name);

    delete_matrix(L, input_size);
    delete_matrix(A, input_size);
    return result;
}

void run_experiment(method_t method, const char* method_name,
                    int input_size, int times) {
    ExperimentResult result;

    for (int i = 0; i < times; ++i) {
        result = experiment(method, method_name, input_size);
        save_results(result);
    }
}

int main(int argc, char* argv[]) {
    int input_sizes[] = {250, 500, 1000, 2500, 5000, 7500};
    int repeat = 10;
    int num_methods = 2;
    int num_sizes = 6;

    method_t methods[] = {
        &cholesky_banachiewicz,
        &cholesky_banachiewicz_avx
    };

    const char* method_names[] = {
        "CB",
        "CB_AVX"
    };

    for (int i = 0; i < num_methods; ++i) {
        for (int j = 0; j < num_sizes; ++j) {
            run_experiment(methods[i], method_names[i], input_sizes[j], repeat);
        }
    }

    return 0;
}