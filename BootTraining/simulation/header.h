#ifndef HEADER_TUYOI_H
#define HEADER_TUYOI_H

//calculate the products of scalar by vector
void product_vec(double t, int size, double *vector_input, double *vector_result);

//calculate the products of scalar by matrix
void product_mat(double t, int rows,int cols , double matrix_input[rows][cols], double matrix_result[rows][cols]);

//calculate the sum of matrixs
void sum_matrix(int rows, int cols, double matrix1[rows][cols], double matrix2[rows][cols], double result[rows][cols]);

//calculete an inverse of an 3*3 inertia tensor I 
void inverse_I(double I[][3], double I_inv[][3]);

//calculate OMEGA for kinematics(in this program quartanion is defined like [lsin msin nsin cos])
void set_OMEGA(double *omega, double (*OMEGA)[4]);

//calcualte dynamics and return dw/dt
void dynamics(double *omega, double (*I)[3], double (*I_inv)[3], double *omega_diff, double *T);

// calcualte kinematics and return dq/dt
void kinematics(double *q, double (*OMEGA)[4], double *q_diff);

//runge dayo 
void rk4(double DT, double *omega, double (*I)[3], double (*I_inv)[3], double *T, double *q);

// calculate banemasu
void banemasu(double DT,double x, double v, double k, double m);

#endif
