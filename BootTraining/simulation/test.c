#include <stdio.h>
#include "header_tuyoi.h"



int main(void){

    double I[3][3] = {
    {1, 0, -1},
    {-2, 1, 0},
    {2, -1, 1}
    } ;

    double I_inv[3][3]={0};

    inverse_I((double **)I,(double **)I_inv);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf(("%lf "),I[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf(("%lf "),I_inv[i][j]);
        }
        printf("\n");
    }
    return 0;
}

