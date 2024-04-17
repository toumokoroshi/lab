#ifndef HEADER_H
#define HEADER_H

/// calcualte dynamics and return dw/dt
void dynamics(double *q1, double *omega){
};

/// calcualte kinematics and return dq/dt
void kinematics(double **inv_I,double *omega,double *){
};

/// discrizate functions and calcualte x_n+1 from x_n
void rk4(double dt,double *x,double t){
};

//calculate the products of scalar by vector
void vector1d(double t, double *vector){
};

//calculate the products of scalar by vector
void vector2d(double t, double *vector){
};

//calculete an inverse of an 4×4 inertia tensor I 
void inverse_I(double **I, double **inv_I){

}
#endif