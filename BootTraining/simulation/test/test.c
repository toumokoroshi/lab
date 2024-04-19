#include <stdio.h>
#include "test.h"

int main() {
    double I[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 10}};
    double I_inv[3][3];
    inverse_I(I, I_inv);

    printf("Inverse of I:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%f ", I_inv[i][j]);
        }
        printf("\n");
    }

    return 0;
}
