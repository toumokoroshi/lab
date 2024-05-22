#line 1 "C:\\lab\\BootTraining\\uryoukei\\test\\LCDtest\\LCDtest.ino"
#include <Arduino.h>

#include <LiquidCrystal.h>

// receiving data format : X,XX,XYY,XYY,XXXY,XXXY,X,XXXX,ZXXY,XXY\r\n
// label            			index	format  unit 	digits(DEC)
// Transmit Counter				1		X	    -	    1
// AnomaryID					2		XX	    -	    2
// solar panel voltage value	3		X.YY	 V	    3
// battery Voltage Value		4		X.YY	 V	    3
// current value				5		XXX.Y	mA	    4
// precipitation				6		XXX.Y   mm      4
// rainfall						7		X		(0|1)	1
// atomospheric preassure		8		XXXX	hPa	    4
// temperature					9		ZXX.Y	℃	   4  Z = 1の時プラス
// humidity						10		XX.Y	%	    3

// esp32 lcdmodule
// 33    14(DB7)
// 25    13(DB6)
// 26    12(DB5)
// 27    11(DB4)
// 14      6(E)
// 12     4(RS)
//        3(VLC)
//        2(Vdd)
//        1(Vss)

const int rs = 15, en = 14, d4 = 27, d5 = 26, d6 = 25, d7 = 33;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#line 32 "C:\\lab\\BootTraining\\uryoukei\\test\\LCDtest\\LCDtest.ino"
void setup();
#line 39 "C:\\lab\\BootTraining\\uryoukei\\test\\LCDtest\\LCDtest.ino"
void loop();
#line 32 "C:\\lab\\BootTraining\\uryoukei\\test\\LCDtest\\LCDtest.ino"
void setup()
{
	lcd.begin(16, 2);
	lcd.print("justice!!!!!");
	delay(3000);
}

void loop()
{
	// set the cursor to column 0, row 0
	lcd.setCursor(0, 0);
	// print the number of seconds since reset:
	lcd.print(millis() / 1000);
}

// void displayLoadingDots()
// {
// 	static int i = 11; // カーソル位置を11から開始

// 	lcd.noCursor();
// 	lcd.backlight();

// 	lcd.setCursor(0, 0);
// 	lcd.print("Please wait");

// 	lcd.setCursor(i, 0);
// 	delay(300);
// 	lcd.print(".");
// 	delay(300);
// 	i++;

// 	if (i == 14)
// 	{
// 		i = 11;
// 		lcd.setCursor(11, 0);
// 		lcd.print("   ");
// 		delay(300);
// 	}
// }

// void displayingandFadingout(char charcter)
// {
// 	char ch1[] = charcter;
// 	int char_count = 0;
// 	lcd.noCursor();
// 	delay(1000);
// 	lcd.backlight();
// 	while (ch1[char_count])
// 	{
// 		lcd.setCursor(char_count, 0);
// 		lcd.print(ch1[char_count]);
// 		delay(100);
// 		char_count++;
// 	}
// 	delay(1000);

// 	int i = 0;
// 	while (i < char_count)
// 	{
// 		lcd.setCursor(i, 0);
// 		lcd.print(" ");
// 		delay(100);
// 		i++;
// 	}
// 	lcd.clear();
// }

// void displayData(char *str, bool datatype)
// {
// 	// display weather data: rainfall, atmospheric pressure, humidity, temperature
	
	
// }


