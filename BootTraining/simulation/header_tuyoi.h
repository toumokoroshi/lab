#ifndef HEADER_H
#define HEADER_H

//calculate the products of scalar by vector
void product_vec(double t, int size, double *vector_input,double *vector_output);

//calculate the products of scalar by matrix
void product_mat(double t, int col, int row, double **matrix_input, double **matrix_output);

//calculate the sum of matrixs
void sum_mat(int col, int row, double **matrix1,double **matrix2,double **matrix_result);

//calculete an inverse of an 3*3 inertia tensor I 
void inverse_I(double I[][3], double I_inv[][3]);

//calculate OMEGA for kinematics(in this program quartanion is defined like [lsin msin nsin cos])
void set_OMEGA(double *omega, double **OMEGA);

//calcualte dynamics and return dw/dt
void dynamics(double *omega1, double **I_inv, double *omega2);

// calcualte kinematics and return dq/dt
void kinematics(double *q1, double **OMEGA, double *q2);

#endif