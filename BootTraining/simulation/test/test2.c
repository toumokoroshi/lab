#include <stdio.h>
#include <math.h>

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

int main() {
    double I[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 10}};
    double I_inv[3][3];
    inverse_I(I, I_inv);

    printf("Inverse of I:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%f ", I_inv[i][j]);
        }
        printf("\n");
    }

    return 0;
}
