#include <LiquidCrystal.h>

/* 
lcd pin asignments
esp32 lcdmodule
28    14(DB7)
27    13(DB6)
23    12(DB5)
13    11(DB4)
9      6(E)
12     4(RS)
       3(VLC)
       2(Vdd)
       1(Vss)
*/

#define DB7 28
#define DB6 27
#define DB5 23
#define DB4 13
#define EN 9
#define RS 12

#define LoRa_Rst 9 //リセットピンをアサイン！！！！！！！！！！！！！！！

LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7);  //create instance of lcd
 
void setup() {
  lcd.begin(16,2);
  lcd.print("Initializing ... ");

  Serial.begin(115200);
}

void loop() {
}

void LoRa_read(){
  if (Serial.available()>0){
    /* code */
  }
}

void LoRa_write(){
  
}

void init_lora(){
// Transition ES920LR to configuration mode
LoRa_write("config\r\n");

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
LoRa_read();

// Select the processor mode of ES920LR
LoRa_write("processor\r\n");
delay(500);

// Restore all settings of ES920LR to default
LoRa_write("load\r\n");
delay(100);

// Set the bandwidth to 125kHz
LoRa_write("bw 4\r\n");
delay(100);

// Set the spreading factor to 7
LoRa_write("sf 7\r\n");
delay(100);

// Set the wireless channel number to 1
LoRa_write("channel 1\r\n");
delay(100);

// Set the PAN network address to ABCD
LoRa_write("panid ABCD\r\n");
delay(100);

// Set the network address of the own node to 500
LoRa_write("ownid 500\r\n");
delay(100);

// Set the network address of the destination node to 0000
LoRa_write("dstid 0000\r\n");
delay(100);

// Save the configured settings to the built-in FlashROM
LoRa_write("save\r\n");
delay(5000);

// Transition ES920LR to operation mode
LoRa_write("start\r\n");
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


