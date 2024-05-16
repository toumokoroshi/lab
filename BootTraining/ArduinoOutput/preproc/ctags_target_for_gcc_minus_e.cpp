# 1 "C:\\lab\\BootTraining\\uryoukei\\main\\main.ino"
# 2 "C:\\lab\\BootTraining\\uryoukei\\main\\main.ino" 2

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
# 23 "C:\\lab\\BootTraining\\uryoukei\\main\\main.ino"
// own(Master Unit) channel number

// bandwidth, 4 means 125MHz

// spreading factor

// PANID : the PAN network address in which the node will participate. The devices to be paired must have the same ID

// OWNID : the network address of the own(Master Unit) node

// the network address of the destination(Slave Unit) node


// create instance of lcd
LiquidCrystal lcd(12, 9, 13, 23, 27, 28);

void setup()
{
  // initialize lcd and display message while initializing lora and SDcardslot
  lcd.begin(16, 2);
  lcd.print("Initializing ... ");

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
  pinMode(9 /* リセットピンをアサイン！！！！！！！！！！！！！！！*/, 0x03);
  delay(500);
  digitalWrite(9 /* リセットピンをアサイン！！！！！！！！！！！！！！！*/, 0x1);
  delay(500);

  // Reset the Private LoRa communication module once
  Serial.println("Reset");
  digitalWrite(9 /* リセットピンをアサイン！！！！！！！！！！！！！！！*/, 0x0);
  delay(500);
  digitalWrite(9 /* リセットピンをアサイン！！！！！！！！！！！！！！！*/, 0x1);
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
  Serial.write("bw BW\r\n");
  delay(100);

  // Set the spreading factor
  Serial.write("sf SF\r\n");
  delay(100);

  // Set the wireless channel number
  Serial.write("channel CH\r\n");
  delay(100);

  // Set the PAN network address
  Serial.write("panid PANID\r\n");
  delay(100);

  // Set the network address of the own node
  Serial.write("ownid OWNID\r\n");
  delay(100);

  // Set the network address of the destination node to 0000
  Serial.write("dstid DSID\r\n");
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
