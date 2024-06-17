#include <omp.h>
#include <stdio.h>
// #include "omp_lib.h"

int main() {
    #pragma omp parallel for
    for (int i = 0; i < 10; i++) {
        printf("the number of threas: %d, i: %d\n", omp_get_thread_num(), i);
    }
    return 0;
}