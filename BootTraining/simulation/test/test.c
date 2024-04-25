#include <stdio.h>
#include <math.h>
#include "C:\Development\lab\BootTraining\simulation\header_tuyoi.h"

int main() {
    double sample_vec[3] = {1, 2, 3};
    double sample_mat1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    double sample_mat2[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };

    double buff1[3] = {0};
    double buff2[3][3] = {0};

    double scalar = 2.0; // スカラーはdouble型として宣言


    product_vec(scalar, 3, sample_vec, buff1);

    printf("product_vecの結果\n");
    for (int i = 0; i < 3; i++)
    {
        printf("%f ", buff1[i]);
    }
    printf("\n");

    int row = sizeof(sample_mat1)/sizeof(sample_mat1[0]);
    int col = sizeof(sample_mat1)/sizeof(sample_mat1[0][0]);

    product_mat(scalar, 3, 3, sample_mat1, buff2);

    printf("product_matの結果\n");
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%f ", buff2[i][j]);
        }
        printf("\n");
    }

    sum_matrix(row, col, sample_mat1, sample_mat2, buff2);

    printf("result of sum_mat\n");
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%f ", buff2[i][j]);
        }
        printf("\n");
    }
    double buff3[4][4] = {0};

    set_OMEGA(sample_vec,buff3);
   
   printf("result of set_OMEGA\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%f ", buff3[i][j]);
        }
        printf("\n");
    }

    return 0;
}
