#ifndef RAINDETECTOR_HANDLER_HPP
#define RAINDETECTOR_HANDLER_HPP

// 雨が降っていると判断するための最小上昇幅 [V]
#define WATER_DETECOTR_RAIN_START_MINIMUM_INCREASE_RATE 0.2f
// 雨が降っていないと判断するための最大上昇幅 [V]
// この最大上昇幅以内の電圧の上昇は減少とみなす
#define WATER_DETECTOR_RAIN_STOP_MAXIMUM_INCREASE_RATE 0.05f
// 雨が降っていないと判断するための、電圧値連続減少回数
#define WATER_DETECTOR_RAIN_STOP_COUNTER 20
//judge rainning or not from water detector voltage value and its change
void isRainingorNot(bool *is_raining,float *wd_voltage,float *pre_wd_voltage,int *wd_decrease_counter);

#endif