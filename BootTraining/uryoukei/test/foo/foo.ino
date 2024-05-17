const int maxArraySize = 10; // 配列の最大サイズを定義
String dataArray[maxArraySize]; // データを格納する配列

void setup(){
    Serial.begin(115200);
    Serial.println("hello world");
    delay(500);
}

void loop(){
    String receivedData = "";
    while (Serial.available())
    {
        char buff = Serial.read();
        receivedData += buff;
    }
    int CRLFindex = receivedData.indexOf("\\");
    receivedData.remove(CRLFindex);
    int dataCount = splitData(receivedData, dataArray, maxArraySize);
    
    // データを配列に格納した後、各データを表示
    for (int i = 0; i < dataCount; i++) {
      Serial.println(dataArray[i]);
    }
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