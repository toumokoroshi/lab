#include <stdio.h>
#include <math.h>
#include<omp.h>

#define epsilon 1.0e-10;
FILE* outputfile;
FILE* myfile;
// double Y[4], Z[4], Z1[4], K[4][6];
double Y[4], Z[4], Z1[4], K[4][6], Y1[4],Y2[4],Y3[4];
double X[4];
double mu = 3.003e-6;
double t_end = 10;
double dt = 0.0001;
double t = 0.0;
double norm1, norm2, norm_SALI1, norm_SALI2, SALI;
double UV1[2], UV2[2], SALI2[2], SALI1[2], W1[2], W2[2];
double x_min = 1.0;
double x_max = 1.01;
double x_step = 0.0001;
double C = 2.999902;
double y_min, y_max;
double k = -1;  ///////k=1:prograde motion, k=-1:retrograde motion
double q1, q2;
double x, y, vy, vx;
double V;
int i, j = 0;
double c1=1/(2*(2-pow(2,1/3)));
double c2=(1-pow(2,1/3))/(2*(2-pow(2,1/3)));
double c3=(1-pow(2,1/3))/(2*(2-pow(2,1/3)));
double c4=1/(2*(2-pow(2,1/3)));
double d1=1/(2-pow(2,1/3));
double d2=-pow(2,1/3)/(2-pow(2,1/3));
double d3=1/(2-pow(2,1/3));
double d4=0.0;


double distance1(double x, double y) {
    q1 = (x + mu) * (x + mu) + y * y;
    q1 = sqrt(q1);
    return q1;
}

double distance2(double x, double y) {
    q2 = (x - 1 + mu) * (x - 1 + mu) + y * y;
    q2 = sqrt(q2);
    return q2;
}


//equation of motion of backward 
void equation_sy(double Y[4], const double c, const double d, double Z[4]){
    //    Z[0]=Y[0]+(+Y[1]+Y[2])*dt*c;
       Z[0]=Y[0]-(+Y[1]+Y[2])*dt*c;
    //    Z[1]=Y[1]+ (-Y[0]+Y[3])*dt*c;
       Z[1]=Y[1]-(-Y[0]+Y[3])*dt*c;
       Z[2]=Y[2]-(+Y[3]-(1-mu)*(Z[0]+mu)/pow(((Z[0]+mu)*(Z[0]+mu)+Z[1]*Z[1]),3./2.)-mu*(Z[0]-1+mu)/pow(((Z[0]-1+mu)*(Z[0]-1+mu)+Z[1]*Z[1]),3./2.))*dt*d;
       Z[3]=Y[3]-(-Y[2]-(1-mu)*(Z[1])/pow(((Z[0]+mu)*(Z[0]+mu)+Z[1]*Z[1]),3./2.)-mu*(Z[1])/pow(((Z[0]-1+mu)*(Z[0]-1+mu)+Z[1]*Z[1]),3./2.))*dt*d;
}

void vector(const double X[4], const double Z[4], double W[2]) {
    for (int i = 0; i < 2; i++) {
        W[i] = Z[i] - X[i];
    }
}

double norm(const double W[2]) {
    double norm = 0;
    for (int i = 0; i < 2; i++) {
        norm += W[i] * W[i];
    }
    norm = sqrt(norm);
    return norm;
}

void unitvector(double W[2], double norm, double UV[2]) {
    for (int i = 0; i < 2; i++) {
        UV[i] = W[i] / norm;
    }
}


int main() {
    printf("\n  **********************************************************************\n");
    printf("  *   Calculator of SALI with sympletic method                         *\n");
    printf("  *                                                                    *\n");
    printf("  *   This source code was written by Yuto Sakurai,                    *\n");
    printf("  *         Department of Aerospace Engineering,  Nagoya University    *\n");
    printf("  *                                                                    *\n");
    printf("  *   updated by TBT                                                   *\n");
    printf("  *  Last Update :                                                     *\n");
    printf("  **********************************************************************\n\n");

    outputfile = fopen("output_new.d", "w");
    if (outputfile == NULL) {
        printf("  Can not open write file");
    }
    printf(" Simulating >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

    x = x_min; 
    int x_mesh_size = (int)((x_max-x_min)/x_step)+1;

    // #pragma omp parallel for
    for (int xloop_counter = 0; xloop_counter < x_mesh_size; xloop_counter++) {

        printf("xloop_counter : %d, x : %f, x_mesh_size : %d\n",xloop_counter, x, x_mesh_size);

        // printf("\b\b\b\b\b\b\b\b\b\b\b");
        // printf(" %d / %d", xloop_counter+1, x_mesh_size);

        y_max = sqrt(0.01 * 0.01 - (x - 1 + mu) * (x - 1 + mu));
        y_min = 0;
        y = y_min;

        int y_mesh_size = (int)(round((y_max-y_min)/x_step)) + 1; //roundしないと丸め誤差の影響で適切にメッシュサイズを評価できない

        // printf("y : %f x_mesh_size : %d\n",y, y_mesh_size);
        // printf("xstep%f\n",x_step);

        // #pragma omp parallel for
        for (int yloop_counter = 0; yloop_counter < y_mesh_size; yloop_counter++) {
            
            // printf("yloop_counter : %d, y : %.10f\n",yloop_counter,y);

            q1 = distance1(x, y);
            q2 = distance2(x, y);
            // printf("q2 : %f ->\n\n",q2);
            if (q2 < 0.00007) {

                printf(" calculation aborted before SALI\n\n");
                y += x_step;
                continue;
            }
            else if ((x * x + y * y + 2 * (1 - mu) / q1 + 2 * mu / q2 + mu * (1 - mu) - C) < 0) {   ////////Out of ZVC

                printf(" calculation aborted before SALI\n\n");
                y += x_step;

                continue;
            }

            V = sqrt(x * x + y * y + 2 * (1 - mu) / q1 + 2 * mu / q2 + mu * (1 - mu) - C);
            vx = -k * V * y / q2;
            vy = k * V * (x - 1 + mu) / q2;

            X[0] = x;
            X[1] = y;
            X[2] = vx;
            X[3] = vy;

            Z[0] = X[0];
            Z[1] = X[1] + epsilon;
            Z[2] = X[2];
            Z[3] = X[3];

            Z1[0] = X[0] + epsilon;
            Z1[1] = X[1];
            Z1[2] = X[2];
            Z1[3] = X[3];
        
        //debug code starts from here

            // for (int i = 0; i < 3; i++)
            // {
            //     printf("X[%d] : %f, ",i,X[i]);
            //     printf("\n");
            // }
            //     printf("\n");

            // for (int i = 0; i < 3; i++)
            // {
            //     printf("Z[%d] : %f, ",i,Z[i]);
            //     printf("\n");
            // }
            //     printf("\n");

            // for (int i = 0; i < 3; i++)
            // {
            //     printf("Z1[%d] : %.10f, ",i,Z1[i]);
            //     printf("\n");
            // }
            //     printf("\n");
            
        //debug code ends here
        
            t = 0.0;
            while (t < t_end) {
                t += dt;
                q2 = distance2(X[0], X[1]);
                if (q2 < 0.00007) {
                    fprintf(outputfile, "%f  %f  0\n");
                    break;
                }else if(q2 > 0.03){
                    break;
                }

                equation_sy(X,c1,d1,Y1);
                equation_sy(Y1,c2,d2,Y2);
                equation_sy(Y2,c3,d3,Y3);
                equation_sy(Y3,c4,d4,X);

                // printf("x0 : %f, x1 : %f\n",X[0],X[1]);


                equation_sy(Z,c1,d1,Y1);
                equation_sy(Y1,c2,d2,Y2);
                equation_sy(Y2,c3,d3,Y3);
                equation_sy(Y3,c4,d4,Z);
                
                equation_sy(Z1,c1,d1,Y1);
                equation_sy(Y1,c2,d2,Y2);
                equation_sy(Y2,c3,d3,Y3);
                equation_sy(Y3,c4,d4,Z1);
            }

            if(t<t_end){
                fprintf(outputfile, "%f  %f  \n", x, y);
                // printf("q2 : %.10f ->>>>>>>>>>>>>>>>>>",q2);
                // printf(" calculation aborted while SALI\n\n");
                y += x_step;
                continue;
            }

            vector(X, Z, W1);
            vector(X, Z1, W2);

            norm1 = norm(W1);
            norm2 = norm(W2);

            unitvector(W1, norm1, UV1);
            unitvector(W2, norm2, UV2);

            for (int i = 0; i < 2; i++) {
                SALI1[i] = UV1[i] + UV2[i];
                SALI2[i] = UV1[i] - UV2[i];
            }

            norm_SALI1 = norm(SALI1);
            norm_SALI2 = norm(SALI2);

            // /*debug code starts from here*/

            //     // printf("sali1 : %f, sali2 : %f\n", norm_SALI1,norm_SALI2);

            // /*debug code ends here*/

            if (norm_SALI1 > norm_SALI2) {
                SALI = norm_SALI2;
            }
            else {
                SALI = norm_SALI1;
            }
        
            /*debug code starts from here*/

                // printf("sali : %f\n\n", SALI);

            /*debug code ends here*/
            
            fprintf(outputfile, "%f  %f  %f\n", x, y, SALI);

            y += x_step;
            
        }
        fprintf(outputfile, "\n");
        x += x_step;
    }
    printf("\n Finish simulation!!!!!!\n");
    fclose(outputfile);

    myfile = popen("gnuplot -persist", "w");
    fprintf(myfile, "unset key\n");
    if (k == 1) {
        fprintf(myfile, "set title 'Prograde motion'\n");
    }
    else if (k == -1) {
        fprintf(myfile, "set title 'Retrograde motion'\n");
    }
    fprintf(myfile, "set size ratio 1 1\n");
    fprintf(myfile, "set xrange[0.99:1.01]\n");
    fprintf(myfile, "set yrange[-0.01:0.01]\n");
    fprintf(myfile, "set cbrange[0:1.6]\n");
    fprintf(myfile, "set xlabel 'x axis'\n");
    fprintf(myfile, "set ylabel 'y axis'\n");
    fprintf(myfile, "set cblabel 'SALI'\n");
    fprintf(myfile, "set pm3d map\n");
    fprintf(myfile, "set terminal png\n");
    fprintf(myfile, "set output 'k= %f ,C= %f _new1.png'\n", k, C);
    fprintf(myfile, "set palette defined (0.0 \"blue\", 0.1 \"green\", 0.2 \"yellow\",0.3 \"red\")\n");
    fprintf(myfile, "splot 'output_new.d' with pm3d\n");
    return 0;
}

