#include <Arduino.h>
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
  // set the cursor to column 0, line 1
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);
}


