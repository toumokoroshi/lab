#include <stdio.h>

struct sample
{
    short counter = 1;
    short anomaryID = 2;    /* アノマリID */
    float SPVoltage = 1.00;   /* 太陽光パネル電圧値 */
    float battVoltage = 2.00; /* 二次電池電圧 */
    float current = 1.00;     /* 電流 */
    float rainfall = 1.00;    /* 雨量 */
    short rain = 1;        /* 雨検知 */
    short pressure = 1013;    /* 気圧 */
    float temp = 20.0;     /* 気温 */
    float humidity = 15.5;    /* 湿度 */
} ;
void main(){
    char* h[5]
}

