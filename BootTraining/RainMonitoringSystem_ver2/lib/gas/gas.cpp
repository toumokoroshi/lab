#include "gas.hpp"

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

void GAS::init() {
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (WiFi.begin(WIFI_SSID, WIFI_PASSWORD) != WL_DISCONNECTED) {
    ESP.restart();
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    wifi_fail_counter++;
    if (wifi_fail_counter == 10) {
      break;
    }
  }
  if (wifi_fail_counter >= 10) {
    Serial.println("Failed connecting to the WiFi network.");
  } else {
    Serial.println("Connected to the WiFi network!");
  }
}

// TODO 時間情報送信
void GAS::send_raw(String str) {
  if (wifi_fail_counter < 10) {
    long start_time = millis();

    Serial.println("Sending data to Google SpreadSheet:");

    String new_str = str;
    new_str.trim();

    // Google Spreadsheet
    String urlFinal = GAS_URL;
    urlFinal += "?data=" + new_str;
    Serial.println(urlFinal);

    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    // getting response from google sheet
    String payload;
    if (httpCode > 0) {
      payload = http.getString();
      Serial.println("Payload: " + payload);
    }
    //---------------------------------------------------------------------
    http.end();

    long end_time = millis();
    Serial.println("Took: " + String(end_time - start_time) + "ms");
  }
}
