#ifndef RAIN_DETECTOR_gas_hpp
#define RAIN_DETECTOR_gas_hpp

#include <Arduino.h>

#define WIFI_SSID "Buffalo-G-503E"
#define WIFI_PASSWORD "v7teb3ruvivhv"

// #define WIFI_SSID ""
// #define WIFI_PASSWORD ""

#define GAS_URL "https://script.google.com/macros/s/AKfycbxNiJSFGQs_RrfLlmJiPBAmeO1yvGS0eMFMzdIwTJIPi4krUmH5iqXnVJvrD6SUv5GF/exec"

class GAS {
 public:
  int wifi_fail_counter = 0;
  void init();
  void send_raw(String str);
};

#endif
