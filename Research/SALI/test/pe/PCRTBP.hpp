/**
 * @file
 * @brief 平面円制限三体問題（Planar Circular Restricted Three-Body Problem : PCRTBP）を扱うクラス
 * @author tabata
 * @date 2024/06/18
 */

#ifndef PCRTBP_H
#define PCRTBP_H

#include <cmath>
#include <iostream>
#include <stdexcept>

/**
 * @brief 平面円制限三体問題（PCRTBP）を扱うクラス
 * @details このクラスはPCRTBPのシミュレーションを行うための機能を提供
 */
class PCRTBP
{
private:
    //! x[2]:回転座標系におけるx,yの組, v[2] : 回転座標系におけるvx,vyの組, q[2]:一般化運動量vx-y,vy+xの組,
    //! v_abs : 回転座標系における速度の大きさ, c_jacobi : や子微積分
    double x[2], v[2], q[2],v_abs, c_jacobi;
    int k;  //k=1:prograde motion, k=-1:retrograde motion
    double mu = 3.003e-6; 
    double dt = 0.0001;
    double c[4] = 
    {
        1/(2*(2-std::pow(2,1.0/3.0))),
        (1-std::pow(2,1.0/3.0))/(2*(2-std::pow(2,1.0/3.0))),
        (1-std::pow(2,1.0/3.0))/(2*(2-std::pow(2,1.0/3.0))),
        1/(2*(2-std::pow(2,1.0/3.0)))
    };
    double d[4] = 
    {
        1/(2-std::pow(2,1.0/3.0)),
        -std::pow(2,1.0/3.0)/(2-std::pow(2,1.0/3.0)),
        1/(2-std::pow(2,1.0/3.0)),
        0.0
    };

public:
    /**
    * @brief コンストラクタ
    * @param x 初期位置のx座標
    * @param y 初期位置のy座標
    * @param k 運動の種類 (1: prograde, -1: retrograde)
    * @param c_jacobi ヤコビ積分
    */
    PCRTBP(double x, double y, int k, double c_jacobi);

    double get_x() const;
    double get_y() const;
    double get_vx() const;
    double get_vy() const;

    void set_x(double x, double y);
    void set_v();
    void set_dt(double dt);

    double calc_r1() const;
    double calc_r2() const;
    void calc_symplectic4();
};

// クラスメソッドの定義

PCRTBP::PCRTBP(double x, double y, int k, double c_jacobi) : k(k), c_jacobi(c_jacobi)
{
    this->x[0] = x;
    this->x[1] = y;

    double r1 = std::sqrt((x + mu) * (x + mu) + y * y);
    double r2 = std::sqrt((x - 1 + mu) * (x - 1 + mu) + y * y);

    double velocity_term = x * x + y * y + 2 * (1 - mu) / r1 + 2 * mu / r2 + mu * (1 - mu) - c_jacobi;
    if (velocity_term < 0)
    {
        throw std::runtime_error("Absolute value of the velocity has a negative value");
    }
    else
    {
        v_abs = std::sqrt(velocity_term);
        this->v[0] = -k * v_abs * y / r2;
        this->v[1] = k * v_abs * (x - 1 + mu) / r2;
        this->q[0] = v[0]-x[1];
        this->q[1] = v[1]+x[0];
    }
}

double PCRTBP::get_x() const { return x[0]; }
double PCRTBP::get_y() const { return x[1]; }
double PCRTBP::get_vx() const { return v[0]; }
double PCRTBP::get_vy() const { return v[1]; }

void PCRTBP::set_x(double x, double y)
{
    this->x[0] = x;
    this->x[1] = y;
}

void PCRTBP::set_v()
{
    // 速度の設定に関する実装が必要な場合に追加
}

void PCRTBP::set_dt(double dt)
{
    this->dt = dt;
}

double PCRTBP::calc_r1() const
{
    return std::sqrt((x[0] + mu) * (x[0] + mu) + x[1] * x[1]);
}

double PCRTBP::calc_r2() const
{
    return std::sqrt((x[0] - 1 + mu) * (x[0] - 1 + mu) + x[1] * x[1]);
}

void PCRTBP::calc_symplectic()
{
    double x_buff0[4], x_buff1[4];
    double q_buff0[4], q_buff1[4];
    double bunbo0, bunbo1;

    //試しになるべく括弧と乗算を減らす形にしてみる
    
    //loop1 
    //初期値からbuff0に値を更新
    x_buff0[0] = x[0] + x[1] * dt * c[0] + q[0] * dt * c[0];
    x_buff0[1] = x[1] - x[0] * dt * c[0] + q[1] * dt * c[0];

    bunbo0 = std::pow(((x_buff0[0] + mu) * (x_buff0[0] + mu) + x_buff0[1] * x_buff0[1]), 3. / 2.);
    bunbo1 = std::pow(((x_buff0[0] - 1 + mu) * (x_buff0[0] - 1 + mu) + x_buff0[1] * x_buff0[1]), 3. / 2.);

    q_buff0[0] = q[0] + q[1] * dt * d[0] - (1 - mu) * (x_buff0[0] + mu) / bunbo0 * dt * d[0] - mu * (x_buff0[0] - 1 + mu) / bunbo1 * dt * d[0];
    q_buff0[1]= q[1] - q[0] * dt * d[0] - (1 - mu) * (x_buff0[1]) / bunbo0 * dt * d[0] - mu * (x_buff0[1]) / bunbo1 * dt * d[0];

    //loop2
    //buff0からbuff1に値を更新
    x_buff1[0] = x_buff0[0] + x_buff0[1] * dt * c[1] + q_buff0[0] * dt * c[1];
    x_buff1[1] = x_buff0[1] - x_buff0[0] * dt * c[1] + q_buff0[1] * dt * c[1];

    bunbo0 = std::pow(((x_buff1[0] + mu) * (x_buff1[0] + mu) + x_buff1[1] * x_buff1[1]), 3. / 2.);
    bunbo1 = std::pow(((x_buff1[0] - 1 + mu) * (x_buff1[0] - 1 + mu) + x_buff1[1] * x_buff1[1]), 3. / 2.);

    q_buff1[0] = q_buff0[0] + q_buff0[1] * dt * d[1] - (1 - mu) * (x_buff1[0] + mu) / bunbo0 * dt * d[1] - mu * (x_buff1[0] - 1 + mu) / bunbo1 * dt * d[1];
    q_buff1[1] = q_buff0[1] - q_buff0[0] * dt * d[1] - (1 - mu) * (x_buff1[1]) / bunbo0 * dt * d[1] - mu * (x_buff1[1]) / bunbo1 * dt * d[1];

    //loop3
    //buff1からbuff0に値を更新
    x_buff0[0] = x_buff1[0] + x_buff1[1] * dt * c[2] + q_buff1[0] * dt * c[2];
    x_buff0[1] = x_buff1[1] - x_buff1[0] * dt * c[2] + q_buff1[1] * dt * c[2];

    bunbo0 = std::pow(((x_buff0[0] + mu) * (x_buff0[0] + mu) + x_buff0[1] * x_buff0[1]), 3. / 2.);
    bunbo1 = std::pow(((x_buff0[0] - 1 + mu) * (x_buff0[0] - 1 + mu) + x_buff0[1] * x_buff0[1]), 3. / 2.);

    q_buff0[0] = q_buff1[0] + q_buff1[1] * dt * d[2] - (1 - mu) * (x_buff0[0] + mu) / bunbo0 * dt * d[2] - mu * (x_buff0[0] - 1 + mu) / bunbo1 * dt * d[2];
    q_buff0[1] = q_buff1[1] - q_buff1[0] * dt * d[2] - (1 - mu) * (x_buff0[1]) / bunbo0 * dt * d[2] - mu * (x_buff0[1]) / bunbo1 * dt * d[2];

    //loop4
    //buff0からbuff1に値を更新
    x_buff1[0] = x_buff0[0] + x_buff0[1] * dt * c[1] + q_buff0[0] * dt * c[1];
    x_buff1[1] = x_buff0[1] - x_buff0[0] * dt * c[1] + q_buff0[1] * dt * c[1];

    bunbo0 = std::pow(((x_buff1[0] + mu) * (x_buff1[0] + mu) + x_buff1[1] * x_buff1[1]), 3. / 2.);
    bunbo1 = std::pow(((x_buff1[0] - 1 + mu) * (x_buff1[0] - 1 + mu) + x_buff1[1] * x_buff1[1]), 3. / 2.);

    q_buff1[0] = q_buff0[0] + q_buff0[1] * dt * d[1] - (1 - mu) * (x_buff1[0] + mu) / bunbo0 * dt * d[1] - mu * (x_buff1[0] - 1 + mu) / bunbo1 * dt * d[1];
    q_buff1[1] = q_buff0[1] - q_buff0[0] * dt * d[1] - (1 - mu) * (x_buff1[1]) / bunbo0 * dt * d[1] - mu * (x_buff1[1]) / bunbo1 * dt * d[1];

    x[0]=x_buff1[0];
    x[1]=x_buff1[1];
    q[0]=q_buff1[0];
    q[1]=q_buff1[1];
}

#endif // PCRTBP_H