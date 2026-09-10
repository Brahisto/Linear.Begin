#include <benchmark/benchmark.h>
#include "Matrix.h"

static void Multiply(benchmark::State & state) {
    int N = state.range(0);
    Matrix A(N), B(N);
    A.init_random();
    B.init_random();
    
    for (auto _ : state) {
        Matrix C = A*B;
        benchmark::DoNotOptimize(C);
    }
}

BENCHMARK(Multiply)->Arg(50)->Arg(100)->Arg(5000);

static void determinant(benchmark::State & state) {
    int N = state.range(0);
    Matrix A(N);
    A.init_random();
    
    for (auto _ : state) {
        A.det();
        benchmark::DoNotOptimize(A);
    }
}

BENCHMARK(determinant)->Arg(5)->Arg(10)->Arg(20);


static void LU(benchmark::State & state) {
    int N = state.range(0);
    Matrix A(N);
    A.init_random();
    
    for (auto _ : state) {
        A.LU_decomposition();
        benchmark::DoNotOptimize(A);
    }
}

BENCHMARK(LU)->Arg(20)->Arg(100)->Arg(1000);

BENCHMARK_MAIN();