#include <benchmark/benchmark.h>
#include "Matrix.h"

static void Multiply(benchmark::State & state) {
    int N = state.range(0);
    squere_Matrix_real A(N), B(N);
    A.init_random();
    B.init_random();
    
    for (auto _ : state) {
        squere_Matrix_real C = A*B;
        benchmark::DoNotOptimize(C);
    }
}

BENCHMARK(Multiply)->Arg(50)->Arg(100)->Arg(500);

static void determinant(benchmark::State & state) {
    int N = state.range(0);
    squere_Matrix_real A(N);
    A.init_random();
    
    for (auto _ : state) {
        A.determinant_2();
        benchmark::DoNotOptimize(A);
    }
}

BENCHMARK(determinant)->Arg(5)->Arg(10)->Arg(20);


static void LU(benchmark::State & state) {
    int N = state.range(0);
    squere_Matrix_real A(N);
    A.init_random();
    
    for (auto _ : state) {
        A.LU_decomposition();
        benchmark::DoNotOptimize(A);
    }
}

BENCHMARK(LU)->Arg(20)->Arg(100)->Arg(500);

BENCHMARK_MAIN();