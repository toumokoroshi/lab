#include <stdio.h>
#include <math.h>
#include "header_tuyoi.h"

//calculate the products of scalar by vector
void product_vec(double t, int size, double *vector_input, double *vector_result){
    for (int i = 0; i < size; i++){
        vector_result[i] = vector_input[i] * t;
    }
}

//calculate the products of scalar by matrix
void product_mat(double t, int rows, int cols, double matrix_input[rows][cols], double matrix_result[rows][cols]){   
    for ( int i = 0; i < rows; i++){
        for ( int j = 0; j < cols; j++){
           matrix_result[i][j] = matrix_input[i][j] * t; 
        }
    }
}

//calculate the sum of matrixs
void sum_matrix(int rows, int cols, double matrix1[rows][cols], double matrix2[rows][cols], double result[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
}

//calculete an inverse of an 3*3 inertia tensor I 

void inverse_I(double I[][3], double I_inv[][3]) {
    const int N = 3;
    const double MAX_ERR = 1e-10;

    /* 逆行列を求める行列用の２次元配列 */
    double sweep[N][N * 2];

    int i, j, k; /* 行、列、注目対角成分が存在する列 */

    double a; /* 定数倍用 */

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            /* sweepの左側に逆行列を求める行列をセット */
            sweep[i][j] = I[i][j];

            /* sweepの右側に単位行列をセット */
            sweep[i][N + j] = (i == j) ? 1 : 0;
        }
    }

    /* 全ての列の対角成分に対する繰り返し */
    for (k = 0; k < N; k++) {

        /* 最大の絶対値を注目対角成分の絶対値と仮定 */
        double max = fabs(sweep[k][k]);
        int max_i = k;

        /* k列目が最大の絶対値となる行を探す */
        for (i = k + 1; i < N; i++) {
            if (fabs(sweep[i][k]) > max) {
                max = fabs(sweep[i][k]);
                max_i = i;
            }
        }

        if (fabs(sweep[max_i][k]) <= MAX_ERR) {
            /* 逆行列は求められない */
            printf("逆行列は求められません...\n");
            return;
        }

        /* 操作（１）：k行目とmax_i行目を入れ替える */
        if (k != max_i) {
            for (j = 0; j < N * 2; j++) {
                double tmp = sweep[max_i][j];
                sweep[max_i][j] = sweep[k][j];
                sweep[k][j] = tmp;
            }
        }

        /* sweep[k][k]に掛けると1になる値を求める */
        a = 1 / sweep[k][k];

        /* 操作（２）：k行目をa倍する */
        for (j = 0; j < N * 2; j++) {
            /* これによりsweep[k][k]が1になる */
            sweep[k][j] *= a;
        }

        /* 操作（３）によりk行目以外の行のk列目を0にする */
        for (i = 0; i < N; i++) {
            if (i == k) {
                /* k行目はそのまま */
                continue;
            }

            /* k行目に掛ける値を求める */
            a = -sweep[i][k];

            for (j = 0; j < N * 2; j++) {
                /* i行目にk行目をa倍した行を足す */
                /* これによりsweep[i][k]が0になる */
                sweep[i][j] += sweep[k][j] * a; 
            }
        }
    }

    /* sweepの右半分がmatの逆行列 */
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            I_inv[i][j] = sweep[i][N + j];
        }
    }
}


//calculate OMEGA for kinematics(in this program quartanion is defined like [lsin msin nsin cos])
void set_OMEGA(double *omega, double (*OMEGA)[4]){
    int size = 4;

    for (int i = 0; i < size; i++)
    {
        OMEGA[i][i] = 0;
    }

    OMEGA[0][1] = omega[2];
    OMEGA[0][2] = -omega[1];
    OMEGA[0][3] = omega[0];
    OMEGA[1][0] = -omega[2];
    OMEGA[1][2] = omega[0];
    OMEGA[1][3] = omega[1];
    OMEGA[2][0] = omega[1];
    OMEGA[2][1] = -omega[0];
    OMEGA[2][3] = omega[2];
    OMEGA[3][0] = -omega[0];
    OMEGA[3][1] = -omega[1];
    OMEGA[3][2] = -omega[2];    
}

/// calcualte dynamics and return dw/dt
void dynamics(double *omega, double (*I)[3], double (*I_inv)[3], double *omega_diff, double *T){
    int count = 3;
    double buff1[3]={0};
    double buff2[3]={0};

    //calculate Iw
    for ( int i = 0; i < count; i++)
    {
        for (int j = 0; j < count; j++)
        {
            buff1[i] += I[i][j]*omega[j];
        } 
    }
    //calculate Iw×w +T
    buff2[0] = buff1[1]*omega[2] - buff1[2]*omega[1] + T[0];
    buff2[1] = buff1[2]*omega[0] - buff1[0]*omega[2] + T[1];
    buff2[2] = buff1[0]*omega[1] - buff1[1]*omega[0] + T[2];

    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count; j++)
        {
            omega_diff[i] += I_inv[i][j]*buff2[j];
        }
    }
    
}

//kinematics
void kinematics(double *q, double (*OMEGA)[4], double *q_diff){
    int count=4;
    for ( int i = 0; i < count; i++)
    {
        q_diff[i] = 0;
        for (int j = 0; j < count; j++)
        {
            q_diff[i] += q[i] * OMEGA[i][j];
        } 
        q_diff[i] *= 0.5;
    }
}

//runge dayo 
void rk4(double DT, double *omega, double (*I)[3], double (*I_inv)[3], double *T, double *q){
    double k1w[3], k2w[3], k3w[3], k4w[3];
    double k1q[4], k2q[4], k3q[4], k4q[4];
    double buff3[3] = {0};
    double buff4[4] = {0};
    double OMEGA[4] = {0};

    //k1
    dinamics(omega, I, I_inv, k1w, T);

    set_OMEGA(omega,OMEGA);     /*OMEGAってどんな感じで更新するっけ*/
    kinematics(q,OMEGA,k1q);
    
    //k2
    product_vec(DT/2,3,k1w,buff3);
    sum_matrix(3,1,omega,buff3,buff3);
    dinamics(buff3, I, I_inv, k2w, T);

    product_vec(DT/2,4,k1q,buff4);
    set_OMEGA(buff3,OMEGA);     /*OMEGAってどんな感じで更新するっけ*/
    sum_matrix(4,1,q,buff4,buff4);
    kinematics(buff4,OMEGA,k2q);

    //k3
    product_vec(DT/2,3,k2w,buff3);
    sum_matrix(3,1,omega,buff3,buff3);
    dinamics(buff3, I, I_inv, k3w, T);

    product_vec(DT/2,4,k2q,buff4);
    set_OMEGA(buff3,OMEGA);     /*OMEGAってどんな感じで更新するっけ*/
    sum_matrix(4,1,q,buff4,buff4);
    kinematics(buff4,OMEGA,k3q);

    //k4
    product_vec(DT,3,k3w,buff3);
    sum_matrix(3,1,omega,buff3,buff3);
    dinamics(buff3, I, I_inv, k4w, T);

    product_vec(DT,4,k3q,buff4);
    set_OMEGA(buff3,OMEGA);     /*OMEGAってどんな感じで更新するっけ*/
    sum_matrix(4,1,q,buff4,buff4);
    kinematics(buff4,OMEGA,k4q);
    
    for (int i = 0; i < 4; i++)
    {
        omega[i] += DT * ( k1w[i] + 2.0 * k2w[i] + 2.0 * k3w[i] + k4w[i] +) / 6.0;
        q[i] += DT * ( k1q[i] +  2.0 * k2q[i] + 2.0 * k3q[i] + k4q[i]) / 6.0;
    }  
}

// calculate banemasu
void banemasu(double DT,double x, double v, double k, double m)
{
    double k1[4], k2[4];

    k1[0] = v;
    k2[0] = -k/m*x;

    k1[1] = v + DT*k1[0]/2;
    k2[1] = -k/m*(v + DT * k2[0]/2);

    k1[2] = v + DT*k1[1]/2;
    k2[2] = -k/m*(v + DT * k2[1]/2);

    k1[3] = v + DT*k1[2];
    k2[3] = -k/m*(v + DT * k2[2]);

    x += DT*(k1[0] + 2*k1[1] + 2*k1[2] + k1[3])/6;
    v += DT*(k2[0] + 2*k2[1] + 2*k2[2] + k2[3])/6;
}

