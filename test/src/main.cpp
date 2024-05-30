#include <Arduino.h>

int RX_PIN = 22;
int TX_PIN = 21;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
}

void loop()
{
  char key;
  String data = "";
  // 受信データがあった時だけ、処理を行う
  while (Serial1.available())
  { // 受信データがあるか？
    key = Serial1.read();
    delay(1); // 1文字だけ読み込む            // 1文字送信。受信データをそのまま送り返す。
    data += key;
    // Serial.print(key);
  }
  if (data == "\r" || data == "\n")
  {
    data = "";
  }
  if (data != "")
  {
    Serial.println(data);
  }

  delay(1000);
}   