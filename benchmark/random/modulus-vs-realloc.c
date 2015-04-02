#include "bench/bench.c"

#include <stdlib.h> // realloc, malloc, free

int main() {

    BENCHMARK(check_realloc, 1)

        int len = 10 * sizeof(char*);
        char* ptr = malloc(len);

        for (int i = 0; i < 12; i++) {
            if (i >= len) continue;

            len += 10 * sizeof(char*);
            ptr = realloc(ptr, len);
        }

        free(ptr);

    END_BENCHMARK(check_realloc)
    BENCHMARK_SUMMARY(check_realloc);

    BENCHMARK(realloc_free, 1)

        char* ptr = malloc(sizeof(char*));

        for (int i = 1; i < 12; i++)
            ptr = realloc(ptr, i * sizeof(char*));

        free(ptr);

    END_BENCHMARK(realloc_free)

    BENCHMARK_SUMMARY(realloc_free);

    return 0;
}