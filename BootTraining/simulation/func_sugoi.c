#import <stdio.h>
#import <math.h>
#import "header_tuyoi.h"

/// calcualte dynamics and return dw/dt
void dynamics(double *omega1, double **Iinvese, double *T,double omega2){
    int count=4;
    for ( int i = 0; i < count; i++)
    {
        for (int j = 0; j < count; j++)
        {
            q2[i] += q1[i] * OMEGA[i][j];
        } 
    }
};

/// calcualte kinematics and return dq/dt
void kinematics(double *q1, double **OMEGA, double T, double *q2){
    int count=4;
    for ( int i = 0; i < count; i++)
    {
        for (int j = 0; j < count; j++)
        {
            q2[i] += q1[i] * OMEGA[i][j];
        } 
    }
};

/// discrizate functions and calcualte x_n+1 from x_n
void rk4(double dt,double *x,double t){
};

//calculate the products of scalar by vector
void product_vec(double t, int size, double *vector_input,double *vector_output){
    for (int i = 0; i < size+1; i++)
    {
        vector_output[i] *= t;
    }
};

//calculate the products of scalar by vector
void product_mat(double t, int col,int row , double **matrix_input, double **matrix_output){
    for ( i = 0; i < col+1; i++)
    {
        for ( j = 0; j < row+1; j++)
        {
           matrix_output[i][j] *= t; 
        }
    }
    
};

//calculete an inverse of an 4×4 inertia tensor I 
void inverse_I(double **I, double **inv_I){
    const int N=3;
    const double MAX_ERR 1e-10;

    /* 逆行列を求める行列用の２次元配列 */
    double mat[N][N];

    /* 逆行列用の２次元配列 */
    double inv[N][N];

    /* 掃き出し法を行う行列 */
    double sweep[N][N * 2];

    int i; /* 行 */
    int j; /* 列 */
    int k; /* 注目対角成分が存在する列 */

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
            return 0;
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