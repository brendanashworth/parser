#include "bench/bench.c"

int main() {

	char i = '1';

    BENCHMARK(ternary, 10)

        int j = (i == '1') ? 1 : '0' - i;

    END_BENCHMARK(ternary)
    BENCHMARK_SUMMARY(ternary);

    BENCHMARK(subtract, 10)

    	int j = '0' - i;

    END_BENCHMARK(subtract)
    BENCHMARK_SUMMARY(subtract);

    return 0;
}
