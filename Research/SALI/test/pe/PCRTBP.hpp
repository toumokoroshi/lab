/**
 * @file 
 * @brief 平面円制限三体問題（Planer Circular Restricted Three-Body Problem : PCRTBP）を扱うクラス
 * @author tabata
 * @date 2024/06/18
 */

#ifndef PCRTBP_H
#define PCRTBP_H

#include <math.h>
#include <iostream>

/**
* @brief 簡単なクラスの説明
* @details 詳細なクラスの説明
*/

class PCRTBP
{
private:
    //! ほにゃららで使う変数
    double x[2],v[2],r1,r2,v_abs,c_jacobi;
    double mu = 3.003e-6; 
    double c[4] = 
    {
        1/(2*(2-pow(2,1/3))),
        (1-pow(2,1/3))/(2*(2-pow(2,1/3))),
        (1-pow(2,1/3))/(2*(2-pow(2,1/3))),
        1/(2*(2-pow(2,1/3)))
    };
    double d[4] = 
    {
        1/(2-pow(2,1/3)),
        -pow(2,1/3)/(2-pow(2,1/3)),
        1/(2-pow(2,1/3)),
        0.0
    };

public:
    /**
    * @brief コンストラクタの簡単な説明
    */
    PCRTBP(double x, double y,double c_jacobi) : x[0](x), x[1](y), c_jacobi(c_jacobi)
    {
        if ((x * x + y * y + 2 * (1 - mu) / r1 + 2 * mu / r2 + mu * (1 - mu) - c_jacobi) < 0)
        {
            throw std::runtime_error("absolute value of the velocity has negative value");
        }
    }
    double get_x();
    double get_y();
    double get_vx();
    double get_vy();

    void set_x(double x, double y);
    void set_v();

    void calc_symplectic();
};

    PCRdouble get_x();
    double get_y();
    double get_vx();
    double get_vy();

    void set_x(double x, double y);
    void set_v();

    void calc_symplectic();


#endif

