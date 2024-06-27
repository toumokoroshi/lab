////////////////////////////////////////////////////////////////////////////////
/// @file           SALI.c
/// @brief          櫻井さんの研究に基づいてSALIの計算を行う
/// @author         tabata
/// @date           ファイル作成年月日
/// $Version:       0.0$
/// $Revision:      0$
/// @note           ファイルに備考などを明記する場合はここへ書き込む
/// @attention      ファイルに注意書きなどを明記する場合はここへ書き込む
/// @par            History
///                 ファイルに履歴などを明記する場合はここへ書き込む
///
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <cstdio>

#include "C:\Development\lab\Research\SALI\test\pe\PCRTBP.hpp"
#include "C:\Development\lab\Research\SALI\test\pe\calc.h"

void progressBar(int progress, int total);
std::string getCurrentDateTime();

int main()
{
    constexpr double epsilon = 1.0e-10;
    constexpr double mu = 3.003e-6;
    constexpr int k = -1;
    constexpr double C_jacobi = 3.000201;
    constexpr double Influence_sphere_radius = 0.03;
    constexpr double forbidden_area_radius = 0.00007;

    std::string dateTime = getCurrentDateTime();
    std::string fileName = dateTime + ".dat";
    // std::string fileName = "sample.dat";
    std::ofstream outFile(fileName);
    if (!outFile)
    {
        std::cerr << "Can't open file : " << fileName << std::endl;
        return -1;
    }

    std::cout << "\n"
        << "simulating ... " << "\n"
        << std::endl;

    double init_x;
    double x_min = 0.99;
    double x_max = 1.01;
    double x_step = 0.0001;

    init_x = x_min;

    int x_mesh_size = (int)std::round((x_max - x_min) / x_step) + 1;

    for (int xloop_counter = 0; xloop_counter < x_mesh_size; ++xloop_counter)
    {

        progressBar(xloop_counter, x_mesh_size);

        double init_y;
        double y_max = std::sqrt(0.01 * 0.01 - (init_x - 1 + mu) * (init_x - 1 + mu));
        double y_min = -y_max;
        // double y_max = 0.0001;
        // double y_min = 0;
        int y_mesh_size = (int)std::round((y_max - y_min) / x_step) + 1;

        init_y = y_min;

        for (int yloop_counter = 0; yloop_counter < y_mesh_size; ++yloop_counter)
        {
            PCRTBP pcrtbp0(init_x, init_y, k, C_jacobi);
            PCRTBP pcrtbp1(init_x + epsilon, init_y, k, C_jacobi);
            PCRTBP pcrtbp2(init_x, init_y + epsilon, k, C_jacobi);

            double r2 = pcrtbp0.calc_r2();
            if (r2 < forbidden_area_radius)
            {
                init_y += x_step;
                continue;
            }

            /*debug codes start from here */

            // std::cout << " initial x ,initial y, initial r2 : " << init_x << ", " << init_y << ", " << r2 << std::endl;
            // std::cout << "initial vx , initial vy = " << pcrtbp0.get_vx() << ", " << pcrtbp0.get_vy() << std::endl;
            // std::cout << "q0 , q1 : " << pcrtbp0.get_q0() << ", " << pcrtbp0.get_q1() << "\n" << std::endl;

            /*debug codes end here */

            double t = 0.0;
            double t_end = 10;
            double dt = pcrtbp0.get_dt();

            while (t < t_end)
            {
                t += dt;
                r2 = pcrtbp0.calc_r2();
                /*debug codes start from here */

                // std::cout << " t = " << t << std::endl;
                // std::cout << " x , y, r2 : " << pcrtbp0.get_x() << ", " << pcrtbp0.get_y() << ", " << r2 << std::endl;
                // std::cout << "vx , vy : " << pcrtbp0.get_vx() << ", " << pcrtbp0.get_vy() << std::endl;
                // std::cout << "q0 , q1 : " << pcrtbp0.get_q0() << ", " << pcrtbp0.get_q1() << "\n" << std::endl;


                /*debug codes end here */
                if (r2 < forbidden_area_radius || r2 > Influence_sphere_radius)
                {
                    break;
                }
                pcrtbp0.symplectic_integration_step();
                pcrtbp1.symplectic_integration_step();
                pcrtbp2.symplectic_integration_step();
            }

            if (t < t_end)
            {
                outFile << init_x << " " << init_y << " " << 16 << std::endl;
                /*debug codes start from here */

                // std::cout << " after x = " << pcrtbp0.get_x() << ", after y = " << pcrtbp0.get_y() << std::endl;
                // std::cout << " after r2 = " << r2 << std::endl;
                // std::cout << "after vx = " << pcrtbp0.get_vx() << ", after vy = " << pcrtbp0.get_vy() << std::endl;
                // std::cout << " after t = " << t << "\n"
                //     << std::endl;

                /*debug codes end here */
                init_y += x_step;
                continue;
            }

            double dev_vec0[2];
            double dev_vec1[2];
            calc_deviation_vector(pcrtbp0.get_xvec(), pcrtbp1.get_xvec(), dev_vec0);
            calc_deviation_vector(pcrtbp0.get_xvec(), pcrtbp2.get_xvec(), dev_vec1);

            /*debug codes start from here */

            // std::cout << " dev_vec0 = " << dev_vec0[0] << ", " << dev_vec0[1] << "\n"
            //     << " dev_vec1 = " << dev_vec1[0] << ", " << dev_vec1[1] << std::endl;
            // std::cout << " after r2 = " << r2 << std::endl;
            // std::cout << " after t = " << t << "\n"
            //     << std::endl;

            /*debug codes end here */
            double dev_vec_unit0[2];
            double dev_vec_unit1[2];
            calc_unit_vector(dev_vec0, dev_vec_unit0);
            calc_unit_vector(dev_vec1, dev_vec_unit1);

            double SALI0[2];
            double SALI1[2];

            for (int i=0; i < 2; i++)
            {
                SALI0[i] = dev_vec_unit0[i] + dev_vec_unit1[i];
                SALI1[i] = dev_vec_unit0[i] - dev_vec_unit1[i];
            }

            double norm_SALI0 = calc_norm(SALI0);
            double norm_SALI1 = calc_norm(SALI1);

            double SALI;
            SALI = (norm_SALI0 > norm_SALI1) ? norm_SALI1 : norm_SALI0;

            /*debug codes start from here */

            // std::cout << " after x = " << pcrtbp0.get_x() << ", after y = " << pcrtbp0.get_y() << std::endl;
            // std::cout << " after r2 = " << r2 << std::endl;
            // std::cout << "after vx = " << pcrtbp0.get_vx() << ", after vy = " << pcrtbp0.get_vy() << std::endl;
            // std::cout << " after t = " << t << std::endl;
            // std::cout << " after SALI = " << SALI << "\n"
            //     << std::endl;

            /*debug codes end here */

            outFile << init_x << " " << init_y << " " << SALI << std::endl;
            // outFile << pcrtbp0.get_x() << " " << pcrtbp0.get_y() << std::endl;
            init_y += x_step;
        }
        outFile << std::endl;
        init_x += x_step;
    }

    outFile.close();
    std::cout << "\n"
        << "Simulation finished" << std::endl;

    // gnuplotコマンドを使うためにファイルを開く
    FILE* myfile = popen("gnuplot -persist", "w");

    if (myfile == nullptr)
    {
        std::cerr << "Failed to open gnuplot." << std::endl;
        return 1;
    }

    // gnuplotコマンドを送信
    fprintf(myfile, "unset key\n");
    if (k == 1)
    {
        fprintf(myfile, "set title 'Prograde motion'\n");
    }
    else if (k == -1)
    {
        fprintf(myfile, "set title 'Retrograde motion'\n");
    }
    fprintf(myfile, "set size ratio 1 1\n");
    fprintf(myfile, "set xrange[0.99:1.01]\n");
    fprintf(myfile, "set yrange[-0.01:0.01]\n");
    fprintf(myfile, "set cbrange[0:1.6]\n");
    fprintf(myfile, "set xlabel 'x axis'\n");
    fprintf(myfile, "set ylabel 'y axis'\n");
    fprintf(myfile, "set cblabel 'SALI'\n");
    fprintf(myfile, "set pm3d map\n");
    fprintf(myfile, "set terminal png\n");
    fprintf(myfile, "set output 'k= %d ,C= %f _new.png'\n", k, C_jacobi);
    fprintf(myfile, "set palette defined (0.0 \"blue\", 0.1 \"green\", 0.2 \"yellow\",0.3 \"red\")\n");
    fprintf(myfile, "splot '%s' with pm3d\n", fileName.c_str());

    // ファイルを閉じる
    pclose(myfile);

    return 0;
}

void progressBar(int progress, int total)
{
    const int barWidth = 70;
    float progressRatio = static_cast<float>(progress) / static_cast<float>(total);
    int barLength = static_cast<int>(progressRatio * barWidth);

    std::cout << "[";
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < barLength)
        {
            std::cout << "=";
        }
        else
        {
            std::cout << " ";
        }
    }
    std::cout << "] " << std::setw(3) << static_cast<int>(progressRatio * 100.0) << "%" << "\r";
    std::cout.flush();
    // if (static_cast<int>(progressRatio * 100.0) > 99)
    //     std::cout << "[";
    // {
    //     for (int i = 0; i < barWidth; ++i)
    //     {

    //         std::cout << "=";
    //     }
    // }
    // std::cout << "] " << std::setw(3) << "100%" << "\r";
    std::cout.flush();
}

std::string getCurrentDateTime()
{
    // 現在の時刻を取得
    std::time_t now = std::time(nullptr);
    std::tm* ltm = std::localtime(&now);

    // 日付時刻をフォーマット
    std::ostringstream oss;
    oss << std::put_time(ltm, "%y%m%d%H%M");
    return oss.str();
}


