#include <string.h>
#include <stdlib.h>
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


//PIN ASIGNMENT
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



//受信データを突っ込む用
//なんでやねん
typedef struct 
{
    short counter;
    short anomaryID;    /* アノマリID */
    float SPVoltage;   /* 太陽光パネル電圧値 */
    float battVoltage; /* 二次電池電圧 */
    float current;     /* 電流 */
    float rainfall;    /* 雨量 */
    short rain;        /* 雨検知 */
    short pressure;    /* 気圧 */
    float temp;     /* 気温 */
    float humidity;    /* 湿度 */
} formedData;

//受信データinputをformedDataの形式にフォーマットする
void splitData(char *input, formedData *dataStruct) {
    // 改行文字を削除
    input[strcspn(input, "\r\n")] = 0;

    sscanf(input, "%hd,%hd,%f,%f,%f,%f,%hd,%hd,%f,%f",
           &dataStruct->counter,
           &dataStruct->anomaryID,
           &dataStruct->SPVoltage,
           &dataStruct->battVoltage,
           &dataStruct->current,
           &dataStruct->rainfall,
           &dataStruct->rain,
           &dataStruct->pressure,
           &dataStruct->temp,  
           &dataStruct->humidity);
    dataStruct->SPVoltage /=100;
    dataStruct->battVoltage /=100;
    dataStruct->current/=10;
    dataStruct->rainfall/=10;
    dataStruct->temp/=10;
    dataStruct->humidity/=10;
}