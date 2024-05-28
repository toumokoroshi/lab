#include <Arduino.h>
#include <SD.h>
#include <LiquidCrystal.h>
#include <setup.hpp>
#include <raindetecter_handler.hpp>
#include <SDmodule.hpp>

float pre_wd_voltage = 0.0;//previous value of water detector volage value
int wd_decrease_counter = 0;

LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7);
byte celsius_degree[8]={   // bit data for celsius degree character
    B11100,
    B10100,
    B11100,
    B01111,
    B01000,
    B01000,
    B01111,
};

formattedData datastruct;  
char receieved_data[44];

bool displayData_weather = true; // true: display weather data, false: display system data

// Interrupt flag
volatile bool dataReceived = false;

// Interrupt handler for serial data reception
void IRAM_ATTR serialEvent1() {
    strcpy(receieved_data,"");
    int i = 0;
    while (Serial.available()) {
        char buff = Serial.read();
        receieved_data[i] = buff;
        i++;
        if (buff == '\n') {
            dataReceived = true;
        } 
    }
}

// Function to check the state of the switch
void checkSwitch() {
    static bool lastSwitchState = HIGH;
    bool currentSwitchState = digitalRead(SWITCH_PIN);
  if (lastSwitchState  ^ currentSwitchState ) {
    displayData_weather = currentSwitchState;
    delay(200); // Debounce delay
  }
}

void setup()
{
    lcd.begin(16, 2);
    lcd.print("Initializing...");
    lcd.createChar(0,celsius_degree);

    // starting Serial com with lora
    Serial.begin(BAUDRATE);
    while (!Serial);
 
    // Set pin mode for the switch
    pinMode(SWITCH_PIN, INPUT_PULLUP);
    
    // starting com with and initializing SDmod
    if(!SD.begin()){
        lcd.print("Card Mount Failed");
        delay(2000);
        return;
    }

    lcd.clear();
    delay(50);
    lcd.print("Get Ready!");
    delay(1000);
    lcd.print("Waiting for data");

    // Set up interrupt
    attachInterrupt(digitalPinToInterrupt(RX), serialEvent1, CHANGE);
}

void loop()
{
    // Check if an interrupt has occurred
    noInterrupts(); // Disable interrupts while checking and resetting the flag
    bool received = dataReceived;
    dataReceived = false;
    interrupts(); // Re-enable interrupts


    //////////////////////add atomospheric pressure handling

    if (received) {
        writeFile(SD, "/data.txt", receieved_data); 
        formatData(receieved_data, &datastruct);
        
        //judge rainning or not from water detector voltage value and its change
        isRainingorNot(&datastruct.is_raining,&datastruct.waterdetector_Voltage,&pre_wd_voltage,&wd_decrease_counter);

        //conbert each data to characters to display
        char rainfallBuff[8];
        char tempBuff[8];
        char preassureBuff[8];
        char humidityBuff[8];
        snprintf(rainfallBuff, sizeof(rainfallBuff), "%4.1f", datastruct.rainfall);
        snprintf(tempBuff, sizeof(tempBuff), "%4.1f", datastruct.temp);
        snprintf(preassureBuff, sizeof(preassureBuff), "%d", datastruct.pressure);
        snprintf(humidityBuff, sizeof(humidityBuff), "%3.1f", datastruct.humidity);

        char counterBuff[8];
        char anomaryIDBuff[8];
        char pvBuff[8];
        char battBuff[8];
        char currentBuff[8];
        snprintf(counterBuff, sizeof(counterBuff), "%d", datastruct.counter);
        snprintf(anomaryIDBuff, sizeof(anomaryIDBuff), "%d", datastruct.anomaryID);
        snprintf(pvBuff, sizeof(pvBuff), "%4d", datastruct.PV_Voltage);
        snprintf(battBuff, sizeof(battBuff), "%3.1f", datastruct.batt_Voltage);
        snprintf(currentBuff, sizeof(currentBuff), "%4.1f", datastruct.current);
        
        // Clear the LCD and display new data
        lcd.clear();
        lcd.setCursor(0, 0);
        static bool datablink = 0;
        if (displayData_weather) {
            if (datastruct.is_raining)
            {
                lcd.print("Rainfall:     mm"); // ５マス分のスペースは降水量の５文字分
                lcd.setCursor(9, 0);
                lcd.print(rainfallBuff);
            } else {
                lcd.print("No rain");
            }

            if (datablink)
            {
                lcd.setCursor(0, 1);
                lcd.print("                ");
                lcd.setCursor(0, 1);
                lcd.print(tempBuff);
                lcd.setCursor(4, 1);
                lcd.write(byte(0));
                lcd.setCursor(8, 1);
                lcd.print(humidityBuff);
                lcd.setCursor(12, 1);
                lcd.print("%");
                delay(2000);
                datablink = !datablink;
            } else {
                lcd.setCursor(0, 1);
                lcd.print("                ");
                lcd.setCursor(0, 1);
                lcd.print(preassureBuff);
                lcd.setCursor(5, 1);
                lcd.print("hpa");
                delay(2000);
                datablink = !datablink;
            }

        } else {
            lcd.setCursor(0, 0);
            lcd.print("PV/Bat:    /");
            lcd.setCursor(7, 0);
            lcd.print(pvBuff);
            lcd.setCursor(12, 0);
            lcd.print(battBuff);
            lcd.setCursor(0, 1);
            lcd.print("I:");
            lcd.setCursor(2, 1);
            lcd.print(currentBuff);
            lcd.setCursor(12, 1);
            lcd.print(counterBuff);
            lcd.setCursor(13, 1);
            lcd.print("/");
            lcd.setCursor(14, 1);
            lcd.print(anomaryIDBuff);
            delay(2000);
            datablink = !datablink;
        }

        checkSwitch();
    }
}
