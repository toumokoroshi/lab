#include <Arduino.h>
#line 1 "C:\\lab\\BootTraining\\uryoukei\\test\\foo\\foo.ino"
#line 1 "C:\\lab\\BootTraining\\uryoukei\\test\\foo\\foo.ino"
void setup();
#line 12 "C:\\lab\\BootTraining\\uryoukei\\test\\foo\\foo.ino"
void loop();
#line 1 "C:\\lab\\BootTraining\\uryoukei\\test\\foo\\foo.ino"
void setup() {
  // ハードウェアシリアル通信を開始
  Serial.begin(9600);
  Serial2.begin(9600);
  while (!Serial) {
    ; // シリアルポートが接続されるまで待つ
  }
  Serial.println("Hardware Serial Initialized");

}

void loop() {
    String sendingdata = "";
    String receiveddata = "";
  // pcからのデータをloraに送信
    while (Serial.available()) {
        char c = Serial.read();
        sendingdata += c;
    }

    if (sendingdata.length())
    {
        Serial2.print(sendingdata + "\r\n");
    }
    
    delay(1000);

    // ソ送信
    while (Serial2.available()) {
        char c = Serial2.read();
        receiveddata += c;
    }

    if (receiveddata.length())
    {
        Serial2.print(receiveddata + "\r\n");
    }
    delay(1000);
}


