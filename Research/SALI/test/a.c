
#include <stdio.h>
#include <math.h>

double mu = 3.003e-6;
double x_min = 1.00;
double x_max = 1.01;
double x_step = 0.0001;
double y_min, y_max;
double x, y;


int main() {
    int x_mesh_size = (int)((x_max-x_min)/x_step) + 1;
    x = x_min;
    
    for (int xloop_counter = 0; xloop_counter < x_mesh_size; xloop_counter++) {
        printf("x : %.10f\n",x);
        y_max = sqrt(0.01 * 0.01 - (x - 1 + mu) * (x - 1 + mu));
        y_min = 0;
        y = y_min;

        int y_mesh_size = (int)((y_max-y_min)/x_step) + 1;

        for (int yloop_counter = 0; yloop_counter < y_mesh_size; yloop_counter++) {
            y +=x_step;
        }
        x +=x_step;
    }
    return 0;
}
