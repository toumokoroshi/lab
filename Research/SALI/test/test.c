#include <stdio.h>
int main() {
    double x_max = 1.01;
    double x_min = 1.00;
    double x_step = 0.001;
    int xloopNum = (int)((x_max-x_min)/x_step);

    printf("xloopNum = %d", xloopNum);

    return 0;
}