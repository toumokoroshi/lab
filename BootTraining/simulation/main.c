#include "header_tuyoi.h"
#include <stdio.h>
#include <stdlib.h>


int main(){
    double omega[3]={0};
    double q[4] ={0};
    double I[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
    } ;
    double T[3] = {0};

    double I_inv[3][3], dt;

    const double t_min = 0;
    const double t_max = 0;
    const int N = 0;

    if (N != 0)
    {
         double dt = (t_max-t_min)/N;
    }
    else
    {
        printf("ZeroDivisionError : dt calculation includes division by zero");
        exit(1);
        return 0;
    }
    
    inverse_I(I,I_inv);

    for (double t = t_min; t <= t_max; t += dt)
    {
        printf("%ld ", t);

        rk4(dt, omega, I, I_inv, T, q);

        for (int i = 0; i < 3; i++){
            printf("%ld ",omega[i]);
        }
        for (int i = 0; i < 4; i++){
            printf("%ld ", q[i]);
        }
        
        printf("\n");
    }
    return 0;
}

