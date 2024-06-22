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

#include "PCRTBP.hpp"

void progressBar(int progress, int total);
std::string getCurrentDateTime();
void calc_deviation_vector(double input_vector0[2], double input_vector1[2],double output_vector[2]);
void calc_unit_vector(double input_vector[2],double output_vector[2]);
double calc_norm(double input_vector[2]);

int main()
{
    constexpr double epsilon = 1.0e-10;
    constexpr double mu = 3.003e-6;
    constexpr int k = -1;
    constexpr double C_jacobi = 3.0000201;
    constexpr double Influence_sphere_radius = 0.03;
    constexpr double forbidden_area_radius = 0.00007;

    std::string dateTime = getCurrentDateTime();
    std::string fileName = dateTime + ".dat";
    std::ofstream outfile;
    if (!outfile)
    {
        std::cerr << "Can't open file : " << fileName << std::endl;
        return -1;
    }

    std::cout << "simulating >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
    std::cout << "\n";

    double init_x;
    double x_min = 0.99;
    double x_max = 1.01;
    double x_step = 0.0001;

    init_x = x_min;

    int x_mesh_size = (int)std::round((x_max - x_min) / x_step);

    for (int xloop_counter = 0; xloop_counter < x_mesh_size; ++xloop_counter)
    {

        progressBar(xloop_counter, x_mesh_size);

        double init_y;
        double y_max = std::sqrt(0.01 * 0.01 - (init_x - 1 + mu) * (init_x - 1 + mu));
        double y_min = -y_max;
        int y_mesh_size = (int)std::round((y_max - y_min) / x_step);

        init_y = y_min;

        for (int yloop_counter = 0; yloop_counter < y_mesh_size; ++yloop_counter)
        {
            PCRTBP pcrtbp0(init_x, init_y, k, C_jacobi);
            PCRTBP pcrtbp1(init_x + epsilon, init_y, k, C_jacobi);
            PCRTBP pcrtbp2(init_x, init_y, k + epsilon, C_jacobi);

            double r2 = pcrtbp0.calc_r2();
            if (r2 < forbidden_area_radius)
            {
                init_y += x_step;
                continue;
            }

            double t = 0.0;
            double t_end = 10;
            double dt = pcrtbp0.get_dt();

            while (t < t_end)
            {
                t += dt;
                r2 = pcrtbp0.calc_r2();
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
                outfile << pcrtbp0.get_x() << " " << pcrtbp0.get_y() << std::endl;
                continue;
            }

            double dev_vec0[2] = {0};
            double dev_vec1[2] = {0};
            calc_deviation_vector(pcrtbp0.get_xvec(), pcrtbp1.get_xvec(), dev_vec0);
            calc_deviation_vector(pcrtbp0.get_xvec(), pcrtbp2.get_xvec(),dev_vec1);

            double dev_vec_unit0[2] = {0};
            double dev_vec_unit1[2] = {0};
            calc_unit_vector(dev_vec0, dev_vec_unit0);
            calc_unit_vector(dev_vec1, dev_vec_unit1);

            double SALI0[2] = {0};
            double SALI1[2] = {0};

            for (int i; i < 2; i++)
            {
                SALI0[i] = dev_vec_unit0[i] + dev_vec_unit1[i];
                SALI1[i] = dev_vec_unit0[i] - dev_vec_unit1[i];
            }

            double norm_SALI0 = calc_norm(SALI0);
            double norm_SALI1 = calc_norm(SALI1);

            double SALI;
            SALI = (norm_SALI0 > norm_SALI1) ? norm_SALI0 : norm_SALI1;

            outfile << pcrtbp0.get_x() << " " << pcrtbp0.get_y() << SALI << std::endl;
            init_y += x_step;
        }
        outfile << std::endl;
        init_x += x_step;
    }

    outfile.close();
    std::cout << "\n"
              << "Simulation finished" << std::endl;

    // gnuplotコマンドを使うためにファイルを開く
    FILE *myfile = popen("gnuplot -persist", "w");

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
    fprintf(myfile, "set cbrange[-1:1.45]\n");
    fprintf(myfile, "set xlabel 'x axis'\n");
    fprintf(myfile, "set ylabel 'y axis'\n");
    fprintf(myfile, "set cblabel 'SALI'\n");
    fprintf(myfile, "set pm3d map\n");
    fprintf(myfile, "set terminal png\n");
    fprintf(myfile, "set output 'k= %d ,C= %f _new1.png'\n", k, C_jacobi);
    fprintf(myfile, "set palette defined (0.0 \"blue\", 0.1 \"green\", 0.2 \"yellow\",0.3 \"red\")\n");
    fprintf(myfile, "splot %s with p\n", fileName.c_str());

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
    std::cout << "] " << std::setw(3) << static_cast<int>(progressRatio * 100.0) << "%\r";
    std::cout.flush();
}

std::string getCurrentDateTime()
{
    // 現在の時刻を取得
    std::time_t now = std::time(nullptr);
    std::tm *ltm = std::localtime(&now);

    // 日付時刻をフォーマット
    std::ostringstream oss;
    oss << std::put_time(ltm, "%y%m%d_%H%M");
    return oss.str();
}

void calc_deviation_vector(double input_vector0[2], double input_vector1[2],double output_vector[2])
{
    output_vector[0] = input_vector0[0] - input_vector1[0];
    output_vector[1] = input_vector0[1] - input_vector1[1];
    return;
}
void calc_unit_vector(double input_vector[2],double output_vector[2])
{
    double n = calc_norm(input_vector);
    for (int i = 0; i < 2; i++)
    {
        output_vector[i] = input_vector[i] / n;
    }
    return;
}

double calc_norm(double input_vector[2])
{
    double norm = sqrt(input_vector[0] * input_vector[0] + input_vector[1] * input_vector[1]);
    return norm;
}
