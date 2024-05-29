#include <stdio.h>
#include <string.h>
#include <setup.hpp>

// 受信データinputをformedDataの形式にフォーマットする
void formatData(char *input, formattedData *dataStruct)
{
    // 改行文字を削除
    input[strcspn(input, "\r\n")] = 0;

    sscanf(input, "%hd,%hd,%f,%f,%f,%f,%f,%hd,%f,%f",
           &dataStruct->counter,
           &dataStruct->anomaryID,
           &dataStruct->PV_Voltage,
           &dataStruct->batt_Voltage,
           &dataStruct->waterdetector_Voltage,
           &dataStruct->current,
           &dataStruct->rainfall,
           &dataStruct->pressure,
           &dataStruct->temp,
           &dataStruct->humidity);
    dataStruct->PV_Voltage /= 100;
    dataStruct->batt_Voltage /= 100;
    dataStruct->waterdetector_Voltage /= 1000;
    dataStruct->current /= 10;
    dataStruct->rainfall /= 10;
    dataStruct->temp = dataStruct->temp >= 1000 ? dataStruct->temp / 10 - 100 : (dataStruct->temp / 10) * -1;
    dataStruct->humidity /= 10;
}
