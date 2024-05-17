void setup(){
    Serial.begin(115200);
    Serial.println("hello world");
    delay(500);
}

void loop(){
    if(Serial.available()>0){
        int incomingByte = Serial.read();
        // Serial.print("with \"print\" : Succeeded to receiving message ");
        // Serial.println("with \"println\" : Succeeded to receiving message ");
        // delay(1000);
        // Serial.print("DEC");
        // Serial.println(incomingByte, DEC);
        String incomingStrings = Serial.readString();
        Serial.print("incoming string is :\"");
        Serial.print(incomingStrings);
        Serial.print("\"");
        delay(100);
        Serial.print("HEX");
        Serial.println(incomingByte, HEX);
        delay(1000);
    }
}
