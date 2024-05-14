#include <stdio.h>
#include "header_tuyoi.h"

int main(){
    double k,m,t, x, v, dt, tmax;
    k = 2.0;
    m = 1.0;
    x = 1.0;
    v = 0.0;
    t = 0.0;
    dt = 0.01;
    tmax = 60;

    FILE *fp;
    fp = fopen("banemasu_runge.csv", "w");
    if(fp==NULL){
        printf("%sCan't open file\n", "banemasu_runge.csv");
        return -1;
    }

    fprintf(fp, "time,x,v\n");

    for ( t = 0; t < tmax; t += dt){
        fprintf(fp, "%lf,%lf,%lf\n",t,x,v);
        banemasu(dt, x, v, k, m);
    }

    fprintf(fp, "%lf,%lf,%lf\n",t,x,v);
    return 0;
}