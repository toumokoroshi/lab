#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 受信データフォーマット用構造体
typedef struct
{
    short counter;                  /*Transmit Counter*/
    short anomaryID;                /* AnomaryID */
    float SP_Voltage;               /* solar panel voltage value */
    float batt_Voltage;             /* battery Voltage Value */
    float waterdetector_Voltage;    /* water detector Voltage Value*/
    float current;                  /* current value */
    float rainfall;                 /* precipitation */
    short pressure;                 /* atomospheric preassure */
    float temp;                     /* temperature */
    float humidity;                 /* humidity */
    bool is_raining;                /* raining -> 1, no rain -> 0*/
} formattedData;



void formatData(char *input, formattedData *datastruct)
{
    // 改行文字を削除
    input[strcspn(input, "\r\n")] = 0;

    sscanf(input, "%hd,%hd,%f,%f,%f,%f,%f,%hd,%f,%f",
           &datastruct->counter,
           &datastruct->anomaryID,
           &datastruct->SP_Voltage,
           &datastruct->batt_Voltage,
           &datastruct->waterdetector_Voltage,
           &datastruct->current,
           &datastruct->rainfall,
           &datastruct->pressure,
           &datastruct->temp,
           &datastruct->humidity
           );
    datastruct->SP_Voltage /= 100;
    datastruct->batt_Voltage /= 100;
    datastruct->waterdetector_Voltage /= 1000;
    datastruct->current /= 10;
    datastruct->rainfall /= 10;
    datastruct->temp = datastruct->temp>=1000? datastruct->temp/10-100 :(datastruct->temp/10) *-1;
    datastruct->humidity /= 10;
}

int main() {

    formattedData datastruct;
    char data[44]="1,00,123,234,1234,1234,3456,6456,0235,647\r\n";

    formatData(data, &datastruct);
    printf("data temp : ");
    for (int i = 33; i < 37; i++)
    {
        printf("%c",data[i]);
    }
    printf("\n");
    printf("Counter: %d\n", datastruct.counter);
    printf("AnomaryID: %d\n", datastruct.anomaryID);
    printf("SPVoltage: %.2f\n", datastruct.SP_Voltage);
    printf("BattVoltage: %.2f\n", datastruct.batt_Voltage);
    printf("water detecter Voltage: %.2f\n", datastruct.waterdetector_Voltage);
    printf("Current: %.1f\n", datastruct.current);
    printf("Rainfall: %.1f\n", datastruct.rainfall);
    printf("Pressure: %d\n", datastruct.pressure);
    printf("Temp: %f\n", datastruct.temp);
    printf("Humidity: %.1f\n", datastruct.humidity);

    return 0;
}