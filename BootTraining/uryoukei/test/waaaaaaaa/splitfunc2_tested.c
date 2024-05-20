#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char data[] = "1,12,345,678,9012,3456,0,7890,+123,456\r\n";

typedef struct 
{
    short counter;
    short anomaryID;    /* アノマリID */
    float SPVoltage;   /* 太陽光パネル電圧値 */
    float battVoltage; /* 二次電池電圧 */
    float current;     /* 電流 */
    float rainfall;    /* 雨量 */
    short rain;        /* 雨検知 */
    short pressure;    /* 気圧 */
    float temp;     /* 気温 */
    float humidity;    /* 湿度 */
} formedData;

void splitData(char *input, formedData *dataStruct) {
    // 改行文字を削除
    input[strcspn(input, "\r\n")] = 0;

    sscanf(input, "%hd,%hd,%f,%f,%f,%f,%hd,%hd,%f,%f",
           &dataStruct->counter,
           &dataStruct->anomaryID,
           &dataStruct->SPVoltage,
           &dataStruct->battVoltage,
           &dataStruct->current,
           &dataStruct->rainfall,
           &dataStruct->rain,
           &dataStruct->pressure,
           &dataStruct->temp,  
           &dataStruct->humidity);
    dataStruct->SPVoltage /=100;
    dataStruct->battVoltage /=100;
    dataStruct->current/=10;
    dataStruct->rainfall/=10;
    dataStruct->temp/=10;
    dataStruct->humidity/=10;
}

int main() {
    formedData dataStruct;
    splitData(data, &dataStruct);

    printf("Counter: %d\n", dataStruct.counter);
    printf("AnomaryID: %d\n", dataStruct.anomaryID);
    printf("SPVoltage: %.2f\n", dataStruct.SPVoltage);
    printf("BattVoltage: %.2f\n", dataStruct.battVoltage);
    printf("Current: %.1f\n", dataStruct.current);
    printf("Rainfall: %.1f\n", dataStruct.rainfall);
    printf("Rain: %d\n", dataStruct.rain);
    printf("Pressure: %d\n", dataStruct.pressure);
    printf("Temp: %.1f\n", dataStruct.temp);
    printf("Humidity: %.1f\n", dataStruct.humidity);

    return 0;
}
