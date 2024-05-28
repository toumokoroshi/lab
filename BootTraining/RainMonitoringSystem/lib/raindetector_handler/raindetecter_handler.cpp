#include <Arduino.h>
#include <raindetecter_handler.hpp>

void isRainingorNot(int *is_raining,float *wd_voltage,float *pre_wd_voltage,int *wd_decrease_counter) {
  float voltage_diff_water_detector =
      *wd_voltage - *pre_wd_voltage;
  *pre_wd_voltage = *wd_voltage;
  if (*is_raining == 0) {
    // 直前：「雨が降っていない」 -> 雨が降り始めたかを確認
    if (voltage_diff_water_detector >
        WATER_DETECOTR_RAIN_START_MINIMUM_INCREASE_RATE) {
      // 雨が降り始めたと判断
      *is_raining = 1;
    }
  } else {
    // 直前：「雨が降っている」 -> 雨が止んだかを確認
    if (voltage_diff_water_detector <
        WATER_DETECTOR_RAIN_STOP_MAXIMUM_INCREASE_RATE) {
      // 雨が降っていない可能性が高いと判断
      *wd_decrease_counter++;
      if (*wd_decrease_counter >= WATER_DETECTOR_RAIN_STOP_COUNTER) {
        // 雨は止んだと判断
        *is_raining = 0;
        *wd_decrease_counter = 0;
      }
    } else {
      // 雨が降り続けていると判断
      *wd_decrease_counter = 0;
    }
  }
//   uint8_t text[16];
//   sprintf(text, "WDVD: %f V\r\n", voltage_diff_water_detector);
//   send_uart(text);
//   sprintf(text, "Rain: %d (%d)\r\n", *is_raining,
//           *wd_decrease_counter);
//   send_uart(text);
}