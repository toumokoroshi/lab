#include <stdio.h>

// バネマスモデルのパラメータ
#define M 1.0  // 質量
#define K 1.0  // ばね定数

// 微分方程式の定義
void derivatives(double x, double v, double *dxdt, double *dvdt) {
    *dxdt = v;
    *dvdt = -K / M * x;
}

// 4次のルンゲクッタ法
void runge_kutta_4(double *x, double *v, double dt) {
    double k1_x, k1_v;
    double k2_x, k2_v;
    double k3_x, k3_v;
    double k4_x, k4_v;
    double x_temp, v_temp;

    derivatives(*x, *v, &k1_x, &k1_v);
    x_temp = *x + 0.5 * dt * k1_x;
    v_temp = *v + 0.5 * dt * k1_v;
    derivatives(x_temp, v_temp, &k2_x, &k2_v);
    x_temp = *x + 0.5 * dt * k2_x;
    v_temp = *v + 0.5 * dt * k2_v;
    derivatives(x_temp, v_temp, &k3_x, &k3_v);
    x_temp = *x + dt * k3_x;
    v_temp = *v + dt * k3_v;
    derivatives(x_temp, v_temp, &k4_x, &k4_v);

    *x += dt / 6.0 * (k1_x + 2.0 * k2_x + 2.0 * k3_x + k4_x);
    *v += dt / 6.0 * (k1_v + 2.0 * k2_v + 2.0 * k3_v + k4_v);
}

int main() {
    // 初期条件
    double x = 1.0;  // 初期変位
    double v = 0.0;  // 初期速度
    double t = 0.0;  // 開始時刻
    double dt = 0.01;  // タイムステップ
    double t_max = 10.0;  // 終了時刻

    // 出力用ファイル
    FILE *fp = fopen("output.csv", "w");
    if (fp == NULL) {
        fprintf(stderr, "ファイルが開けませんでした。\n");
        return 1;
    }

    // 時刻歴解析
    while (t <= t_max) {
        // 結果をファイルに出力
        fprintf(fp, "%f,%f,%f\n", t, x, v);

        // ルンゲクッタ法で次のステップを計算
        runge_kutta_4(&x, &v, dt);

        // 時刻を更新
        t += dt;
    }

    // ファイルを閉じる
    fclose(fp);

    return 0;
}
