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

void displayLoadingDots() {
  static int i = 11; // カーソル位置を11から開始
  
  lcd.noCursor();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Please wait");
  
  lcd.setCursor(i, 0);
  delay(300);
  lcd.print(".");
  delay(300);
  i++;
  
  if (i == 14) {
    i = 11;
    lcd.setCursor(11, 0);
    lcd.print("   ");
    delay(300);
  }
}

void displayingandFadingout(char charcter){
    char ch1[]= charcter;
    int char_count=0;
    lcd.noCursor();
    delay(1000);
    lcd.backlight();
    while(ch1[char_count]){
      lcd.setCursor(char_count,0);
      lcd.print(ch1[char_count]);
      delay(100);
      char_count++;
    }  
    delay(1000);

    int i=0;
    while(i<char_count){
      lcd.setCursor(i,0);
      lcd.print(" ");
      delay(100);
      i++;    
    }
    lcd.clear();   
}

void displayWeatherData(char *str){
	
}

void displayDebaggData(){}

