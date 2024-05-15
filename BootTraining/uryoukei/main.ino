#include <LiquidCrystal.h>

// esp32 lcdmodule
// 28    14(DB7)
// 27    13(DB6)
// 23    12(DB5)
// 13    11(DB4)
// 9      6(E)
// 12     4(RS)
//        3(VLC)
//        2(Vdd)
//        1(Vss)

#define DB7 28
#define DB6 27
#define DB5 23
#define DB4 13
#define EN 9
#define RS 12

LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7);
 
void setup() {
  lcd.begin(16,2);
  lcd.print("hello, world!");
}

void loop() {
  
}


// Function to divide data by comma and store them in an array
int splitData(String data, String* dataArray, int maxArraySize) {
  int dataCount = 0;
  int startIndex = 0;
  int endIndex = data.indexOf(',');

  while (endIndex != -1 && dataCount < maxArraySize) {
    dataArray[dataCount++] = data.substring(startIndex, endIndex);
    startIndex = endIndex + 1;
    endIndex = data.indexOf(',', startIndex);
  }

  if (dataCount < maxArraySize) {
    dataArray[dataCount++] = data.substring(startIndex);
  }

  return dataCount;
}

