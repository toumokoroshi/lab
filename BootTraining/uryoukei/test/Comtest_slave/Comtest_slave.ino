#define LoRa_Rst 9 // リセットピンをアサイン！！！！！！！！！！！！！！！
// own(Master Unit) channel number
#define CH "1"
// bandwidth, 4 means 125MHz
#define BW "4"
// spreading factor
#define SF "7"
// PANID : the PAN network address in which the node will participate. The devices to be paired must have the same ID
#define PANID "0000"
// OWNID : the network address of the own(Master Unit) node
#define OWNID "0000"
// the network address of the destination(Slave Unit) node
#define DSID "0001" 

void setup(){
  // opens serial port for uart with lora
  Serial.begin(115200);
}

void loop(){
}

// Function to initialize lora
void init_lora(){

  // Transition ES920LR to configuration mode
  Serial.write("config\r\n");

  // Define the pin to reset the Private LoRa communication module
  pinMode(LoRa_Rst, OUTPUT);
  delay(100);
  digitalWrite(LoRa_Rst, HIGH);
  delay(100);

  // Reset the Private LoRa communication module once
  Serial.println("Reset");
  digitalWrite(LoRa_Rst, LOW);
  delay(500);
  digitalWrite(LoRa_Rst, HIGH);
  delay(500);

  // Receive messages from the Private LoRa communication module
  // String response = "";
  // while (true) {
  //   if (Serial.available()>0) {
  //       continue;
  //     }
  //   break;
  //   }
  //   delay(10);

  while (Serial.available()>0) {
      char c = Serial.read();
      if (response.endsWith("\r\n")) {
        break;
    }
  }
  delay(10);
  
  // Select the processor mode of ES920LR
  Serial.write("processor\r\n");
  waitForOK();

  // Restore all settings of ES920LR to default
  Serial.write("load\r\n");
  waitForOK();

  // Set the bandwidth
  Serial.write("bw %s\r\n", BW);
  waitForOK();

  // Set the spreading factor
  Serial.write("sf %s\r\n", sf);
  waitForOK();

  // Set the wireless channel number
  Serial.write("channel %s\r\n", CH);
  waitForOK();

  // Set the PAN network address
  Serial.write("panid %s\r\n", PANID);
  waitForOK();

  // Set the network address of the own node
  Serial.write("ownid %s\r\n", OWNID);
  waitForOK();

  // Set the network address of the destination node to 0000
  Serial.write("dstid %s\r\n", DSID);
  waitForOK();

  // Save the configured settings to the built-in FlashROM
  Serial.write("save\r\n");
  waitForOK();

  // Transition ES920LR to operation mode
  Serial.write("start\r\n");
  waitForOK();
}

// Function to wait for "OK\r\n" response from lora
bool waitForOK() {
// void waitForOK() {
  String response = "";
  while (true) {
    if (Serial.available()) 
    {
      char c = Serial.read();
      response += c;
      if (response.endsWith("OK\r\n")) 
      {
        return true;
        // break;
      }
    }
    delay(10);
  }
  return false;
}

// Function to divide data by comma and store them in an array
int splitData(String data, String *dataArray, int maxArraySize)
{
  int dataCount = 0;
  int startIndex = 0;
  int endIndex = data.indexOf(',');

  while (endIndex != -1 && dataCount < maxArraySize)
  {
    dataArray[dataCount++] = data.substring(startIndex, endIndex);
    startIndex = endIndex + 1;
    endIndex = data.indexOf(',', startIndex);
  }

  if (dataCount < maxArraySize)
  {
    dataArray[dataCount++] = data.substring(startIndex);
  }
  return dataCount;
}
