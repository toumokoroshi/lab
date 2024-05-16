#include <LiquidCrystal.h>

/* lcd pin asignments
esp32 lcdmodule
28    14(DB7)
27    13(DB6)
23    12(DB5)
13    11(DB4)
9      6(E)
12     4(RS)
       3(VLC)
       2(Vdd)
       1(Vss)*/

#define DB7 28
#define DB6 27
#define DB5 23
#define DB4 13
#define EN 9
#define RS 12

#define LoRa_Rst 9 //リセットピンをアサイン！！！！！！！！！！！！！！！
//own channel number for lora
#define CH 1
//bandwidth, 4 means 125MHz
#define BW 4

//create instance of lcd
LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7); 
 
void setup() {
//initializing lcd and display "initializing ... " while initializing lora and SDcardslot  
  lcd.begin(16,2);
  lcd.print("Initializing ... ");

//opens serial port for uart with lora
  Serial.begin(115200);
}

void loop() {
}


void init_lora(){
String temp;

// Transition ES920LR to configuration mode
Serial.write("config\r\n");

// Define the pin to reset the Private LoRa communication module
pinMode(LoRa_Rst, OUTPUT);
delay(500);
digitalWrite(LoRa_Rst, HIGH);
delay(500);

// Reset the Private LoRa communication module once
Serial.println("Reset");
digitalWrite(LoRa_Rst, LOW);
delay(500);
digitalWrite(LoRa_Rst, HIGH);
delay(500);

// Receive messages from the Private LoRa communication module
temp = Serial.readString();

// Select the processor mode of ES920LR
Serial.write("processor\r\n");
delay(500);

// Restore all settings of ES920LR to default
Serial.write("load\r\n");
delay(100);

// Set the bandwidth to 125kHz
Serial.write("bw BW\r\n");
delay(100);

// Set the spreading factor to 7
Serial.write("sf 7\r\n");
delay(100);

// Set the wireless channel number to 1
Serial.write("channel CH\r\n");
delay(100);

// Set the PAN network address to ABCD
Serial.write("panid ABCD\r\n");
delay(100);

// Set the network address of the own node to 500
Serial.write("ownid 500\r\n");
delay(100);

// Set the network address of the destination node to 0000
Serial.write("dstid 0000\r\n");
delay(100);

// Save the configured settings to the built-in FlashROM
Serial.write("save\r\n");
delay(5000);

// Transition ES920LR to operation mode
Serial.write("start\r\n");
delay(500);
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


