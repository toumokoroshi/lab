#include <WiFi.h>
#include <time.h>
#include "sntp.h"
#include <sys/time.h>

const char* ssid = "Buffalo-A-61FE";
const char* password = "8c344ur8vs55v";

const char* ntpServer1 = "ntp.nict.jp";
const char* ntpServer2 = "time.google.com";
const char* ntpServer3 = "ntp.jst.mfeed.ad.jp";
const long gmtOffset_sec = 9 * 3600; // 日本のタイムゾーン (GMT+9)
const int daylightOffset_sec = 0;

bool timeset = false;
const unsigned long ntpInterval = 3600000; // 1時間 (3600000ms)
unsigned long previousMillis = 0;

// コールバック関数の定義
void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
  timeset = true;
}

// 現在時刻表示
void showLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.printf("%04d/%02d/%02d %02d:%02d:%02d\n", 
                timeinfo.tm_year + 1900, 
                timeinfo.tm_mon + 1, 
                timeinfo.tm_mday, 
                timeinfo.tm_hour, 
                timeinfo.tm_min, 
                timeinfo.tm_sec);
}

void setup() {
  Serial.begin(115200);
  delay(10);

  // WiFi接続処理
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  unsigned long startAttemptTime = millis();
  const unsigned long wifiTimeout = 30000; // 30秒のタイムアウト
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < wifiTimeout) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi");
    return; // WiFi接続に失敗した場合はsetup関数を終了
  }

  Serial.println("\nConnected to WiFi");

  // NTP設定
  sntp_set_time_sync_notification_cb(timeavailable); // コールバック関数を設定
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2, ntpServer3);

  // 初回の時刻同期
  previousMillis = millis();
  Serial.print("Waiting for time sync");
  while (!timeset && millis() - previousMillis < 30000) { // 30秒のタイムアウト
    delay(1000);
    Serial.print(".");
  }

  if (!timeset) {
    Serial.println("Failed to sync time");
    return; // 時刻同期に失敗した場合はsetup関数を終了
  }

  Serial.println("\nTime synchronized");
  showLocalTime();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= ntpInterval) {
    previousMillis = currentMillis;
    timeset = false;
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2, ntpServer3);
    while (!timeset) {
      delay(1000);
    }
    Serial.println("Time updated");
    showLocalTime();
  }
}
