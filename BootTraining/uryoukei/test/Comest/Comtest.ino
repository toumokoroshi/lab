#define LoRa_Rst 9 // リセットピンをアサイン！！！！！！！！！！！！！！！
// own(Master Unit) channel number
#define CH "1"
// bandwidth, 4 means 125MHz
#define BW "4"
// spreading factor
#define SF "7"
// PANID : the PAN network address in which the node will participate. The devices to be paired must have the same ID
#define PANID "0001"
// OWNID : the network address of the own(Master Unit) node
#define OWNID "0001"
// the network address of the destination(Slave Unit) node
#define DSID "0000" 

void setup(){
  // opens serial port for uart with lora
  Serial.begin(115200);
}

void loop()
{
}

// Function to initialize lora
void init_lora()
{
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

  // Set the bandwidth
  Serial.write("bw %s\r\n", BW);
  delay(100);

  // Set the spreading factor
  Serial.write("sf %s\r\n", sf);
  delay(100);

  // Set the wireless channel number
  Serial.write("channel %s\r\n", CH);
  delay(100);

  // Set the PAN network address
  Serial.write("panid %s\r\n", PANID);
  delay(100);

  // Set the network address of the own node
  Serial.write("ownid %s\r\n", OWNID);
  delay(100);

  // Set the network address of the destination node to 0000
  Serial.write("dstid %s\r\n", DSID);
  delay(100);

  // Save the configured settings to the built-in FlashROM
  Serial.write("save\r\n");
  delay(5000);

  // Transition ES920LR to operation mode
  Serial.write("start\r\n");
  delay(100);
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
