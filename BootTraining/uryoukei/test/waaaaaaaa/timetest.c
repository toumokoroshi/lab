#include <stdio.h>
#include <stdbool.h>
#include <time.h>

const long  gmtOffset_sec = 9 * 3600; // 日本のタイムゾーン (GMT+9)
const int   daylightOffset_sec = 0;

void showLocalTime()
{
  char str[256];
  static const char *wd[7] = { "日", "月", "火", "水", "木", "金", "土" };
  unsigned long m;

  // timeとlocaltimeを使用して現在時刻取得
  time_t t;
  struct tm *tm;
  t = time(NULL);
  tm = localtime(&t);
  
  sprintf(str, "[time localtime] %04d/%02d/%02d(%s) %02d:%02d:%02d : %d", tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, wd[tm->tm_wday], tm->tm_hour, tm->tm_min, tm->tm_sec);
    printf(str);
    printf("\n");
}

void main(){
    showLocalTime();
}