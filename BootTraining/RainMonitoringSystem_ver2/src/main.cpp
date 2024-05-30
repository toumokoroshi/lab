#include <Arduino.h>
#include <SD.h>
#include <LiquidCrystal.h>
#include <setup.hpp>
#include <raindetecter_handler.hpp>
#include <SDmodule.hpp>

// #define RX_PIN 3
// #define TX_PIN 1
#define RX_PIN 22
#define TX_PIN 21

float pre_wd_voltage = 0.0; // previous value of water detector volage value
int wd_decrease_counter = 0;

LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7);
byte celsius_degree[8] = {
    // bit data for celsius degree character
    B11100,
    B10100,
    B11100,
    B01111,
    B01000,
    B01000,
    B01111,
};

char rainfallBuff[8];
char tempBuff[8];
char preassureBuff[8];
char humidityBuff[8];
char counterBuff[8];
char anomaryIDBuff[8];
char pvBuff[8];
char battBuff[8];
char currentBuff[8];

formattedData datastruct;
char receieved_data[44];

bool displayData_weather = true; // true: display weather data, false: display system data

// Interrupt flag
volatile bool dataReceived = false;
String strBuff="";
static bool conbertchecker = 0;

// Interrupt handler for serial data reception
void IRAM_ATTR serialEvent1(){
    //debag section starts from here
    Serial.println("wariokomi\n");
    //debag section ends here
    while (Serial1.available())
    {
        char inChar = (char)Serial1.read();
        strBuff += inChar;
        if (inChar == '\n')
        {
            dataReceived = true;
            conbertchecker = 1;
            break; // Exit the loop once a complete message is received
        }
    }
}

// Function to check the state of the switch
void checkSwitch()
{
    static bool lastSwitchState = HIGH;
    static unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50; // 50ms debounce delay
    bool currentSwitchState = digitalRead(SWITCH_PIN);

    if (currentSwitchState != lastSwitchState)
    {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        if (currentSwitchState != displayData_weather)
        {
            displayData_weather = currentSwitchState;
        }
    }

    lastSwitchState = currentSwitchState;
}

void setup()
{
    lcd.begin(16, 2);
    lcd.print("Initializing...");
    lcd.createChar(0, celsius_degree);

    Serial.begin(BAUDRATE);
    Serial1.begin(BAUDRATE, SERIAL_8N1, RX_PIN, TX_PIN);
    while (!Serial1)
        ;
    // if (!SD.begin()) {
    //     lcd.clear();
    //     lcd.print("Card Mount Failed");
    //     return;
    // }
    // uint8_t cardType = SD.cardType();

    // if (cardType == CARD_NONE) {
    //     lcd.clear();
    //     lcd.print("No SD card attached");
    //     return;
    // }

    pinMode(SWITCH_PIN, INPUT);

    lcd.clear();
    lcd.print("Get Ready!");
    delay(1000);
    lcd.clear();
    lcd.print("Waiting for data");

    attachInterrupt(digitalPinToInterrupt(RX_PIN), serialEvent1, CHANGE);
}

void loop()
{
    noInterrupts();
    bool received = dataReceived;
    dataReceived = false;
    interrupts();

    if (received)
    {
        if (conbertchecker)
        {
            strBuff.toCharArray(receieved_data, sizeof(receieved_data));
            formatData(receieved_data, &datastruct);
            isRainingorNot(&datastruct.is_raining, &datastruct.waterdetector_Voltage, &pre_wd_voltage, &wd_decrease_counter);

            //debag section starts from here
            //debag output to serial monitor
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

            //debag section ends here

            strcpy(tempBuff,"");
            strcpy(preassureBuff,"");
            strcpy(humidityBuff,"");
            strcpy(counterBuff,"");
            strcpy(anomaryIDBuff,"");
            strcpy(pvBuff,"");
            strcpy(battBuff,"");
            strcpy(currentBuff,"");
            snprintf(rainfallBuff, sizeof(rainfallBuff), "%4.1f", datastruct.rainfall);
            snprintf(tempBuff, sizeof(tempBuff), "%4.1f", datastruct.temp);
            snprintf(preassureBuff, sizeof(preassureBuff), "%d", datastruct.pressure);
            snprintf(humidityBuff, sizeof(humidityBuff), "%3.1f", datastruct.humidity);
            snprintf(counterBuff, sizeof(counterBuff), "%d", datastruct.counter);
            snprintf(anomaryIDBuff, sizeof(anomaryIDBuff), "%d", datastruct.anomaryID);
            snprintf(pvBuff, sizeof(pvBuff), "%3.1f", datastruct.PV_Voltage);
            snprintf(battBuff, sizeof(battBuff), "%3.1f", datastruct.batt_Voltage);
            snprintf(currentBuff, sizeof(currentBuff), "%4.1f", datastruct.current);
            conbertchecker = 0;
            
            Serial.print("rainfall : ");
            Serial.println(rainfallBuff);
            Serial.print("temp : ");
            Serial.println(tempBuff);
            Serial.print("preassure : ");
            Serial.println(preassureBuff);
            Serial.print("humidity : ");
            Serial.println(humidityBuff);
            Serial.print("counter : ");
            Serial.println(counterBuff);
            Serial.print("anomaryID : ");
            Serial.println(anomaryIDBuff);
            Serial.print("pv voltage : ");
            Serial.println(pvBuff);
            Serial.print("battery buff : ");
            Serial.println(battBuff);
            Serial.print("current value : ");
            Serial.println(currentBuff);

            strBuff ="";
            strcpy(receieved_data,"");

            if(datastruct.is_raining){
                digitalWrite(LED_PIN, HIGH);
            }else{
                digitalWrite(LED_PIN, LOW);
            }

            // writeFile(SD, "/test.txt", receieved_data);
        }


    }
    if (displayData_weather)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        static bool datablink = 0;
        if (datastruct.is_raining)
        {
            lcd.print("Rainfall:     mm");
            lcd.setCursor(9, 0);
            lcd.print(rainfallBuff);
        }
        else
        {
            lcd.print("No rain");
        }

        if (datablink)
        {
            lcd.setCursor(0, 1);
            lcd.print(tempBuff);
            lcd.write(byte(0));
            lcd.setCursor(8, 1);
            lcd.print(humidityBuff);
            lcd.print("%");
        }
        else
        {
            lcd.setCursor(0, 1);
            lcd.print(preassureBuff);
            lcd.print("hpa");
        }
        datablink = !datablink;
    }
    else
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("PV/Bat:    /");
        lcd.setCursor(7, 0);
        lcd.print(pvBuff);
        lcd.setCursor(12, 0);
        lcd.print(battBuff);
        lcd.setCursor(0, 1);
        lcd.print("I:");
        lcd.print(currentBuff);
        lcd.setCursor(12, 1);
        lcd.print(counterBuff);
        lcd.print("/");
        lcd.print(anomaryIDBuff);
    }

    delay(2000);
    checkSwitch();
}
