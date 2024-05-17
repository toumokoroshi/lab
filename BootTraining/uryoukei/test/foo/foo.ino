void setup(){
    Serial.begin(115200);
    Serial.println("hello world");
    delay(500);
}

void loop(){
    if(Serial.available()>0){
        int incomingByte = Serial.read();
        Serial.print("with \"print\" : Succeeded to receiving message ");
        // Serial.println("with \"println\" : Succeeded to receiving message ");
        delay(1000);
        Serial.println(incomingByte, DEC);
        Serial.println(incomingByte, HEX);
        delay(5000);
    }
}