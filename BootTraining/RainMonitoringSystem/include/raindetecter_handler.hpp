#include <Arduino.h>
// todo WATER DETECTOR to OYAKI
// 雨が降っていると判断するための最小上昇幅 [V]
#define WATER_DETECOTR_RAIN_START_MINIMUM_INCREASE_RATE 0.2f
// 雨が降っていないと判断するための最大上昇幅 [V]
// この最大上昇幅以内の電圧の上昇は減少とみなす
#define WATER_DETECTOR_RAIN_STOP_MAXIMUM_INCREASE_RATE 0.05f
// 雨が降っていないと判断するための、電圧値連続減少回数
#define WATER_DETECTOR_RAIN_STOP_COUNTER 20

// 雨検知電圧値[V] -> X.YYY [V]
float voltage_water_detector = 0.0;

void onDataReceived();