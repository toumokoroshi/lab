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

   _____________________
  |                     |
  |    ESP32-DEVKITC    |
  |                     |
  |  23 MOSI ───────────┬───────── CMD 3
  |  19 MISO ───────────┴───────── DAT0 7
  |  18 SCLK ───────────┬───────── CLK 5
  |   4 CS   ───────────┴───────── CD/DAT3 2
  |   +3.3V ────────────┬───────── VDD 4
  |   GND   ────────────┴───────── VSS 6
  |_____________________|
*/

#ifndef CONFIG_HPP
#define CONFIG_HPP

#define DB7 17
#define DB6 16
#define DB5 15 
#define DB4 14
#define EN 33
#define RS 27
#define LED_PIN 12
#define SWITCH_PIN 13
#define BAUDRATE 115200

// 受信データフォーマット用構造体
typedef struct
{
    short counter;                  /*Transmit Counter*/
    short anomaryID;                /* AnomaryID */
    float PV_Voltage;               /* solar panel voltage value */
    float batt_Voltage;             /* battery Voltage Value */
    float waterdetector_Voltage;    /* water detector Voltage Value*/
    float current;                  /* current value */
    float rainfall;                 /* precipitation */
    short pressure;                 /* atomospheric preassure */
    float temp;                     /* temperature */
    float humidity;                 /* humidity */
    bool is_raining;                /* raining -> 1, no rain -> 0*/
} formattedData;

void formatData(char *input, formattedData *dataStruct);

#endif