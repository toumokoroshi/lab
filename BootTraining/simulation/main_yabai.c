#include <stdio.h>
#include <stdlib.h>
#include "header_tuyoi.h"

void main(){
    double I[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
    } ;

    double k1,k2,k3,k4,dt,list_q[][5], list_omega[][4];

    double q1[4] ={0};
    double q2[4]={0};
    double I_inv[3][3] = {0};
    double OMEGA[3][3] = {0};
    double omega[3]={0};
    double omega_div[3]={0};

    const double t_min = 0;
    const double t_max = 0;
    const int N = 0;

    if (N!=0)
    {
         double dt = (t_max-t_min)/N;
    }
    else
    {
        printf("including zero divide at calculating dt");
        exit(1);
        return 0;
    }
    
    inverse_I(I,I_inv);

    for ( i = 0; i < N; i++)
    {
        dynamics(omega,I,I_inv,omega_div);
        k1 = dt * omega_div;
    }
    

}
