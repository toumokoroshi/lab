

// receiving data format : X,XX,XYY,XYY,XXXY,XXXY,X,XXXX,ZXXY,XXY\r\n

// label            			index	format  unit 	digits(DEC)
// Transmit Counter				1		X	    -	    1
// AnomaryID					2		XX	    -	    2
// solar panel voltage value	3		X.YY	V	    3
// battery Voltage Value		4		X.YY	V	    3
// water detector voltage value	5		X.YYY	V       4
// current value				6		XXX.Y	mA	    4
// precipitation				7		XXX.Y   mm      4
// atomospheric preassure		8		XXXX	hPa	    4
// temperature					9		ZXX.Y	℃	   4  Z = 1の時プラス
// humidity						10		XX.Y	%	    3
//                                                      total : 43digits


/*
           3V3 - |o3V3      GNDo| -
               - |oEN        23o| - MOSI
               - |oVP        22o| -
               - |oVN        TXo| - TX
               - |o34        RXo| - RX
               - |o35        21o| -
               - |o32       GNDo| -
            EN - |o33        19o| - MISO
               - |o25        18o| - CLK
               - |o26         5o| -
            RS - |o27        17o| - DB7
           DB4 - |o14        16o| - DB6
           LED - |o12         4o| - CS
           GND - |oGND        0o| -
    DATASWITCH - |o13         2o| -
               - |oD2        15o| - DB5
               - |oD3        D1o| -
               - |oCMD       D0o| -
               - |o5V       CLKo| -
*/

#include <Arduino.h>
#include <stdlib.h>
#include <string.h>
#include <LiquidCrystal.h>
#include <SDmodule.hpp>

// 受信データフォーマット用構造体
typedef struct
{
    short counter;                  /*Transmit Counter*/
    short anomaryID;                /* AnomaryID */
    float SP_Voltage;               /* solar panel voltage value */
    float batt_Voltage;             /* battery Voltage Value */
    float waterdetector_Voltage;    /* water detector Voltage Value*/
    float current;                  /* current value */
    float rainfall;                 /* precipitation */
    short pressure;                 /* atomospheric preassure */
    float temp;                     /* temperature */
    float humidity;                 /* humidity */
    bool is_raining;                /* raining -> 1, no rain -> 0*/
} formattedData;



void formatData(char *input, formattedData *datastruct)
{
    // 改行文字を削除
    input[strcspn(input, "\r\n")] = 0;

    sscanf(input, "%hd,%hd,%f,%f,%f,%f,%f,%hd,%f,%f",
           &datastruct->counter,
           &datastruct->anomaryID,
           &datastruct->SP_Voltage,
           &datastruct->batt_Voltage,
           &datastruct->waterdetector_Voltage,
           &datastruct->current,
           &datastruct->rainfall,
           &datastruct->pressure,
           &datastruct->temp,
           &datastruct->humidity
           );
    datastruct->SP_Voltage /= 100;
    datastruct->batt_Voltage /= 100;
    datastruct->waterdetector_Voltage /= 1000;
    datastruct->current /= 10;
    datastruct->rainfall /= 10;
    datastruct->temp = datastruct->temp>=1000? datastruct->temp/10-100 :(datastruct->temp/10) *-1;
    datastruct->humidity /= 10;
}

formattedData datastruct;
char data[44]="1,00,123,234,1234,1234,3456,1013,1235,647\r\n";
const int DB7 = 17, DB6 = 16, DB5 = 15, DB4 = 14, EN = 33, RS = 27; // for LCDmodule                                 // serial(default) -lora, serial1 -SD
const int LED_PIN = 12;
const int DATASWITCH_PIN = 13;
const int BAUDRATE = 115200; // for serial communication

float pre_wd_voltage = 0.0;//previous value of water detector volage value
int wd_decrease_counter = 0;

byte celcius_degree[8]={
    B11100,
    B10100,
    B11100,
    B01111,
    B01000,
    B01000,
    B01111,
};

LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7);

void setup()
{
    Serial.begin(BAUDRATE);
    lcd.begin(16,2);
    lcd.print("Hello world!!!!");
    lcd.createChar(0,celcius_degree);
    delay(2000);
    lcd.clear();
    formatData(data, &datastruct);
    datastruct.is_raining = 1;

    char tempBuff[8];
    snprintf(tempBuff, sizeof(tempBuff), "%4.1f", datastruct.temp);
    char preassureBuff[8];
    snprintf(preassureBuff, sizeof(preassureBuff), "%d", datastruct.pressure);
    char humidityBuff[8];
    snprintf(humidityBuff, sizeof(humidityBuff), "%3.1f", datastruct.humidity);

    while (1)
    {
        lcd.setCursor(0, 0);
        if (datastruct.is_raining){
            char buff[16];
            lcd.print("Rainfall:     mm"); // ５マス分のスペースは降水量の５文字分
            lcd.setCursor(9, 0);
            snprintf(buff, sizeof(buff), "%4.1f", datastruct.rainfall);
            lcd.print(buff);
        }
        if (!datastruct.is_raining){
            lcd.print("No rain");
        }
        lcd.setCursor(0, 1);
        lcd.print(tempBuff);
        lcd.setCursor(4, 1);
        lcd.write(byte(0));
        lcd.setCursor(8, 1);
        lcd.print(humidityBuff);
        lcd.setCursor(12, 1);
        lcd.print("%");
        delay(2000);
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(preassureBuff);
        lcd.setCursor(5, 1);
        lcd.print("hpa");
        delay(2000);
        lcd.setCursor(0, 1);
        lcd.print("                ");
    }
}

    // if(digitalRead(DATASWITCH_PIN)==LOW)
    // {
    //     lcd.setCursor(0, 0);
    //     lcd.print("PV/Bat:    /");
    //     char buff[8];
    //     //太陽光パネル電圧
    //     snprintf(buff,sizeof(buff),"%.1f",datastruct.PV_Voltage);
    //     lcd.setCursor(7,0);
    //     lcd.print(buff);
    //     //バッテリ電圧表示
    //     snprintf(buff,sizeof(buff),"%.1f",datastruct.batt_Voltage);
    //     lcd.setCursor(12,0);
    //     lcd.print(buff);
    // }
void loop()
{
    
}