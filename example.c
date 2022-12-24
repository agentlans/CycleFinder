#include <stdio.h>
#include "CycleFinder.h"

// gcc example.c ./libcyclefinder.so -lboost_coroutine -lboost_context

int main() {
    cycf finder = cycf_new();
    cycf_add(finder, 0, 0);
    cycf_add(finder, 0, 1);
    cycf_add(finder, 0, 2);
    cycf_add(finder, 1, 2);
    cycf_add(finder, 2, 0);
    cycf_add(finder, 2, 1);
    cycf_add(finder, 2, 2);
    // After done adding edges, set the graph.
    cycf_set(finder);
    // Find cycles
    size_t cycle_size;
    while (!cycf_finished(finder)) {
        cycf_find_cycle(finder);
        int* cycle = cycf_get_cycle(finder, &cycle_size);
        // Print the cycle
        printf("Found cycle of length %ld: ", cycle_size);
        for (int i = 0; i < cycle_size; ++i) {
            printf("%d ", cycle[i]);
        }
        printf("\n");
    }
    cycf_free(finder);
}