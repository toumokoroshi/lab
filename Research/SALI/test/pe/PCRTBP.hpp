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
    //! 状態変数
    double x[2], v[2], v_abs, c_jacobi;
    double mu = 3.003e-6; 
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
    * @param c_jacobi ヤコビ積分
    */
    PCRTBP(double x, double y, double c_jacobi);

    double get_x() const;
    double get_y() const;
    double get_vx() const;
    double get_vy() const;

    void set_x(double x, double y);
    void set_v();

    double calc_r1() const;
    double calc_r2() const;
    void calc_symplectic();
};

// クラスメソッドの定義

PCRTBP::PCRTBP(double x, double y, double c_jacobi) : c_jacobi(c_jacobi)
{
    this->x[0] = x;
    this->x[1] = y;

    double r1 = std::sqrt((x+mu)*(x+mu)+y*y); 
    double r2 = std::sqrt((x-1+mu)*(x-1+mu)+y*y);

    if ((x * x + y * y + 2 * (1 - mu) / r1 + 2 * mu / r2 + mu * (1 - mu) - c_jacobi) < 0)
    {
        throw std::runtime_error("absolute value of the velocity has negative value");
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
    // 速度の設定に関する実装
    // 要る？
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
    // シンプレクティック法による計算の実装
}

#endif // PCRTBP_H
