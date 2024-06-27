#include <stdio.h>
#include <math.h>

int main(){
    double c = (1 - pow(2, 1. / 3.)) / (2 * (2 - pow(2, 1. / 3.)));
    printf("%f",c);
    return 0;
}