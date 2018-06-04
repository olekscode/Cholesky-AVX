#include "dotprod.h"

typedef struct {
    double* a;
    double* b;
    int begin;
    int end;
} DotprodArgs;

double dotprod(double* a, double* b, int begin, int end) {
    double sum = 0;

    for (int i = begin; i < end; ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}

void* dotprod_wrap(void* args_void) {
    DotprodArgs* args = static_cast<DotprodArgs*>(args_void);

    double* a = (*args).a;
    double* b = (*args).b;
    int begin = (*args).begin;
    int end = (*args).end;

    double sum = dotprod(a, b, begin, end);
    
    double* ret = new double;
    *ret = sum;
    pthread_exit(ret);
}

double dotprod_par(double* a, double* b, int begin, int end) {
    int middle = begin + (end - begin) / 2;

    DotprodArgs args;
    args.a = a;
    args.b = b;
    args.begin = begin;
    args.end = middle;

    pthread_t tid;

    if(pthread_create(&tid, NULL, &dotprod_wrap, &args)) {
        throw ErrorCreatingThread();
    }

    double sum1 = dotprod(a, b, middle, end);
    void* ret_void;

    if(pthread_join(tid, (void**) &ret_void)) {
        throw ErrorJoiningThread();
    }

    double* sum2_ptr = static_cast<double*>(ret_void);
    double sum2 = *sum2_ptr;
    delete sum2_ptr;

    return sum1 + sum2;
}

double dotprod_avx_step(const double* const a, const double* const b) {
    double sum;
    __m256d apd = _mm256_loadu_pd(&a[0]);
    __m256d bpd = _mm256_loadu_pd(&b[0]);
    __m256d cpd = _mm256_mul_pd(apd, bpd);
    __m256d hsum = _mm256_add_pd(cpd,
        _mm256_permute2f128_pd(cpd, cpd, 0x1));
    _mm_store_sd(&sum,
        _mm_hadd_pd(
            _mm256_castpd256_pd128(hsum),
            _mm256_castpd256_pd128(hsum)));
    return sum;
}

double dotprod_avx(double* a, double* b, int begin, int end) {
    double sum = 0;

    for (int i = begin; i + 4 <= end; i += 4) {
        sum += dotprod_avx_step(a + i, b + i);
    }

    for (int i = end % 4; i > 0; --i) {
        sum += a[end - i] * b[end - 1];
    }
    return sum;
}

void* dotprod_avx_wrap(void* args_void) {
    DotprodArgs* args = static_cast<DotprodArgs*>(args_void);

    double* a = (*args).a;
    double* b = (*args).b;
    int begin = (*args).begin;
    int end = (*args).end;

    double sum = dotprod_avx(a, b, begin, end);
    
    double* ret = new double;
    *ret = sum;
    pthread_exit(ret);
}

// TODO: Remove copy-pasted code
double dotprod_avx_par(double* a, double* b, int begin, int end) {
    int middle = begin + (end - begin) / 2;

    DotprodArgs args;
    args.a = a;
    args.b = b;
    args.begin = begin;
    args.end = middle;

    pthread_t tid;

    if(pthread_create(&tid, NULL, &dotprod_avx_wrap, &args)) {
        throw ErrorCreatingThread();
    }

    double sum1 = dotprod_avx(a, b, middle, end);
    void* ret_void;

    if(pthread_join(tid, (void**) &ret_void)) {
        throw ErrorJoiningThread();
    }

    double* sum2_ptr = static_cast<double*>(ret_void);
    double sum2 = *sum2_ptr;
    delete sum2_ptr;

    return sum1 + sum2;
}