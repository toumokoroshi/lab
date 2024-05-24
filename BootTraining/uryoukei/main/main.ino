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

//PIN ASIGNMENT
//TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**
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
#include <string.h>
#include <stdlib.h>
#include <LiquidCrystal.h>

void formatData(char *input, formedData *dataStruct) {};
void writeFile(fs::FS &fs, const char * path, const char * message){};

const int DB7 = 28, DB6 = 27, DB5 = 23 , DB4 = 13, EN = 9 ,RS = 12; //for LCDmodule
const int RX1_PIN = , TX1_PIN = ;  //serial(default) -lora, serial1 -SD
const int LED_PIN = ;
const int DATASWITCH_PIN = ;
const int BRATE = 115200;          //for serial communication

//TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**
//TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**TBD**


LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
formattedData formattedData;
char* data[43];

void setup(){
    //starting Serial com
    Serial.begin(bRate);
    Serial1.begin(115200, SERIAL_8N1, RX1_PIN, TX1_PIN);

    //initialize lcdmod
    lcd.begin(16, 2);
    lcd.print("Initializing...");

    //starting com with and initializing SDmod
    if(!SD.begin()){
        lcd.clear();
        delay(100);
        lcd.print("Card Mount Failed");
        return;
    }

    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        lcd.clear();
        delay(100);
        Serial.println("No SD");
        return;
    }

    lcd.clear();
    delay(50);
    lcd.print("Get Ready!");
    delay(2000); 
    lcd.clear();
}

void loop(){
	while(!Serial.available()){
		;
	}
	
	int i = 0;
	while (Serial.available()){
        	char buff = Serial.read();
        	data[i] = buff;
			i++;
    	}
	formmatData(data,&formattedData);

	if (DATASWITCH_PIN==1)
	{
		/* code */
	}
	

}

//受信データを突っ込む用
//なんでやねん
typedef struct 
{
    short counter;/*Transmit Counter*/
    short anomaryID;    		/* AnomaryID */
    float SPVoltage;   			/* solar panel voltage value */
    float battVoltage;  		/* battery Voltage Value */
    float rainDetectorVoltage;  /*rain detector Voltage Value*/
    float current;     			/* current value */
    float rainfall;    			/* precipitation */
    short pressure;    			/* atomospheric preassure */
    float temp;     			/* temperature */
    float humidity;    			/* humidity */
	bool raindetect;			/*detected rain -> 1, no rain -> 0*/
} formattedData;

//受信データinputをformedDataの形式にフォーマットする
void formatData(char *input, formedData *dataStruct) {
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
           &dataStruct->humidity
		   );
    dataStruct->SPVoltage /=100;
    dataStruct->battVoltage /=100;
	dataStruct->rainDetectorVoltage /=1000
    dataStruct->current/=10;
    dataStruct->rainfall/=10;
    dataStruct->temp/=10;
    dataStruct->humidity/=10;
}