#include "bench/bench.c"

int function_call() {
    return 0;
}

int main() {

    BENCHMARK(function, 1)

        int i = function_call();

    END_BENCHMARK(function)
    BENCHMARK_SUMMARY(function);

    return 0;
}
