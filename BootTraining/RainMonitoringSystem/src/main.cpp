
// receiving data format : X,XX,XYY,XYY,XXXY,XXXY,X,XXXX,ZXXY,XXY\r\n

// label            			index	format  unit 	digits(DEC)
// Transmit Counter				1		X	    -	    1
// AnomaryID					2		XX	    -	    2
// solar panel voltage value	3		X.YY	V	    3
// battery Voltage Value		4		X.YY	V	    3
// rainfall						5		X.YYY	V       4
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
#include <LiquidCrystal.h>

void formatData(char *input, formedData *dataStruct) {};
void writeFile(fs::FS &fs, const char *path, const char *message) {};

const int DB7 = 28, DB6 = 27, DB5 = 23, DB4 = 13, EN = 9, RS = 12; // for LCDmodule                                 // serial(default) -lora, serial1 -SD
const int LED_PIN = 12;
const int DATASWITCH_PIN = 13;
const int BAUDRATE = 115200; // for serial communication
extern const int CLK;
extern const int MISO;
extern const int MOSI;
extern const int CS;

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

// 受信データフォーマット用構造体
typedef struct
{
    short counter;             /*Transmit Counter*/
    short anomaryID;           /* AnomaryID */
    float SPVoltage;           /* solar panel voltage value */
    float battVoltage;         /* battery Voltage Value */
    float rainDetectorVoltage; /*rain detector Voltage Value*/
    float current;             /* current value */
    float rainfall;            /* precipitation */
    short pressure;            /* atomospheric preassure */
    float temp;                /* temperature */
    float humidity;            /* humidity */
    bool raindetect;           /*detected rain -> 1, no rain -> 0*/
} formattedData;

formattedData datastruct;
char data[44];

// 受信データinputをformedDataの形式にフォーマットする
void formatData(char *input, formedData *dataStruct)
{
    // 改行文字を削除
    input[strcspn(input, "\r\n")] = 0;

    sscanf(input, "%hd,%hd,%f,%f,%f,%f,%f,%hd,%f,%f",
           &dataStruct->counter,
           &dataStruct->anomaryID,
           &dataStruct->SPVoltage,
           &dataStruct->battVoltage,
           &dataStruct->rainDetectorVoltage,
           &dataStruct->current,
           &dataStruct->rainfall,
           &dataStruct->pressure,
           &dataStruct->temp,
           &dataStruct->humidity);
    dataStruct->SPVoltage /= 100;
    dataStruct->battVoltage /= 100;
    dataStruct->rainDetectorVoltage /= 1000 dataStruct->current /= 10;
    dataStruct->rainfall /= 10;
    dataStruct->temp /= 10;
    dataStruct->humidity /= 10;
}

void setup()
{
    lcd.begin(16, 2);
    lcd.print("Initializing...");
    lcd.createChar(0,celcius_degree);

    // starting Serial com with lora
    Serial.begin(BAUDRATE);
    //////////////////////////////////////////////////add error handring
    while (!Serial)
    {
        delay(10);
    }

    //////////////////////////////////////////////////add error handring
    while (!SD.begin())
    {
        delay(10);
    }
    // starting com with and initializing SDmod
    // if (!SD.begin())
    // {
    //     lcd.clear();
    //     delay(100);
    //     lcd.print("Card Mount Failed");
    //     return;
    // }

    uint8_t cardType = SD.cardType();

    // if (cardType == CARD_NONE)
    // {
    //     lcd.clear();
    //     delay(10);
    //     Serial.println("No SD");
    //     return;
    // }

    lcd.clear();
    delay(50);
    lcd.print("Get Ready!");
    delay(2000);
    lcd.clear();
}

void loop()
{
    while (!Serial.available())
    {
        delay(10);
    }

    int i = 0;
    while (Serial.available() && i < 43) {
        char buff = Serial.read();
        data[i] += buff;
        i++;
    }
    data[i] = '\0'; // null terminator to mark end of string
    formatData(data, &datastruct);

    ////////////////////////////add rainfall volate handling
    //////////////////////add atomospheric pressure handling
    if (digitalRead(DATASWITCH_PIN) == HIGH)
    {
        lcd.setCursor(0, 0);
        if (raindetect)
        {
            char buffer[16];
            lcd.print("Rainfall:     mm"); // ５マス分のスペースは降水量の５文字分
            lcd.setCursor(9, 0);
            snprintf(buffer, sizeof(buffer), "%.1f", datastruct.rainfall);
            lcd.print(buffer);
        }
        if (!raindetect)
        {
            lcd.print("No rain");
        }
        lcd.setCursor(0, 1);
        char tempBuffer[8];
        snprintf(tempBuffer, sizeof(tempBuffer), "%.2f C", datastruct.temp);
        lcd.print(tempBuffer);
    }

    if(digitalRead(DATASWITCH_PIN)==LOW)
    {
        lcd.setCursor(0, 0);
        lcd.print("PV/Bat:    /");
        char buffer[4];
        //太陽光パネル電圧
        snprintf(buffer,sizeof(buffer),"%.1f",datastruct.SPVoltage);
        lcd.setCursor(7,1);
        lcd.print(buffer);
        //バッテリ電圧表示
        snprintf(buffer,sizeof(buffer),"%.1f",datastruct.battVoltage);
        lcd.setCursor(12,1);
        lcd.print(buffer);
        
    }
}

℃