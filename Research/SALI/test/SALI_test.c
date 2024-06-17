#include <stdio.h>
#include <math.h>
#include<omp.h>

double Y[4], Z[4], Z1[4], K[4][6], Y1[4],Y2[4],Y3[4];
double X[4];
double mu = 3.003e-6;
double x_min = 1.00;
double x_max = 1.01;
double x_step = 0.0001;
double y_min, y_max;


double distance1(double x, double y) {
    double q1 = (x + mu) * (x + mu) + y * y;
    q1 = sqrt(q1);
    return q1;
}

double distance2(double x, double y) {
    double q2 = (x - 1 + mu) * (x - 1 + mu) + y * y;
    q2 = sqrt(q2);
    return q2;
}


int main() {
    printf(" Simulating >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

        double x = x_min; 
        y_max = sqrt(0.01 * 0.01 - (x - 1 + mu) * (x - 1 + mu));
        y_min = 0;
        double y = y_min;

        int y_mesh_size = (int)(round((y_max-y_min)/x_step)) + 1; //roundしないと丸め誤差の影響で適切にメッシュサイズを評価できない

        printf("xstep%f\n",x_step);

        printf("ymax : %f, ymin : %f, xstep : %f\n",y_max,y_min,x_step);
        printf("(y_max-y_min)/x_step : %f,(int)(round((y_max-y_min)/x_step)) + 1 : %d, y_mesh_size : %d\n\n",(y_max-y_min)/x_step,(int)(round((y_max-y_min)/x_step)) + 1,y_mesh_size);
        printf("y : %f y_mesh_size : %d\n",y, y_mesh_size);
    for (int yloop_counter = 0; yloop_counter < y_mesh_size; yloop_counter++) {
            
            printf("yloop_counter : %d, y : %f\n",yloop_counter,y);

            double q1 = distance1(x, y);
            double q2 = distance2(x, y);
            printf("q2 : %f ->\n\n",q2);
            y += x_step;
    }
  
    return 0;
}

