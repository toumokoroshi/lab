#include <WiFi.h>

const char* ssid     = "xxxxxx";
const char* password = "xxxxxx";

const char* ntpServer1 = "ntp.nict.jp";
const char* ntpServer2 = "time.google.com";
const char* ntpServer3 = "ntp.jst.mfeed.ad.jp";
const long  gmtOffset_sec = 9 * 3600;
const int   daylightOffset_sec = 0;

bool timeset = 0;

// 現在時刻表示
void showLocalTime()
{
  char str[256];
  static const char *wd[7] = { "日", "月", "火", "水", "木", "金", "土" };
  unsigned long m;

  // timeとlocaltimeを使用して現在時刻取得
  time_t t;
  struct tm *tm;
  m = millis();
  t = time(NULL);
  tm = localtime(&t);
  sprintf(str, "[time localtime] %04d/%02d/%02d(%s) %02d:%02d:%02d : %d (ms)", tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, wd[tm->tm_wday], tm->tm_hour, tm->tm_min, tm->tm_sec, millis()-m);
  Serial.println(str);

  // getLocalTimeを使用して現在時刻取得
  struct tm timeInfo;
  m = millis();
  getLocalTime(&timeInfo);  
  sprintf(str, "[getLocalTime  ] %04d/%02d/%02d(%s) %02d:%02d:%02d : %d (ms)", timeInfo.tm_year+1900, timeInfo.tm_mon+1, timeInfo.tm_mday, wd[timeInfo.tm_wday], timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec, millis()-m);
  Serial.println(str);
}

void timeavailable(struct timeval *t)
{
  Serial.println("Got time adjustment from NTP!");
  timeset = 1;
}

void setup()
{
  Serial.begin(115200);
  delay(10);

  // 時刻設定前の時刻取得
  Serial.println("<<時刻設定前の時刻取得>>");
  for(int i=0;i<3;i++) {
    showLocalTime();
    delay(1000);
  }

  // Wifi接続処理
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  // NTP
  sntp_set_time_sync_notification_cb( timeavailable );
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2, ntpServer3);

  // 時刻設定後の時刻取得
  while( !timeset ) {
    delay(1000);
  }
  Serial.println("<<時刻設定後の時刻取得>>");
  for(int i=0;i<3;i++) {
    showLocalTime();
    delay(1000);
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
}