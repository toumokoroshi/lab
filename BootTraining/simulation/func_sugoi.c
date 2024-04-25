#include <stdio.h>
#include <math.h>
#include "header_tuyoi.h"

//calculate the products of scalar by vector
void product_vec(double t, int size, double *vector_input,double *vector_output){
    for (int i = 0; i < size; i++){
        vector_output[i] = vector_input[i] * t;
    }
}

//calculate the products of scalar by matrix
void product_mat(double t, int rows,int cols , double matrix[rows][cols], double result[rows][cols]){   
    for ( int i = 0; i < rows; i++){
        for ( int j = 0; j < cols; j++){
           result[i][j] = matrix[i][j]*t; 
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
    buff2[0] = buff1[1]*omega[2] - buff1[2]*omega[1]+T[0];
    buff2[1] = buff1[2]*omega[0] - buff1[0]*omega[2] + T[1];
    buff2[2] = buff1[0]*omega[1] - buff1[1]*omega[0] + T[2];

    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count; j++)
        {
            omega_diff[i] += I[i][j]*buff2[j];
        }
    }
    
}

/// calcualte kinematics and return dq/dt
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


