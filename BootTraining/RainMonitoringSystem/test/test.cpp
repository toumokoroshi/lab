    if (received) {
        // Clear the LCD and display new data
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Received:");
        lcd.setCursor(0, 1);
        if (displayData_weather) {
            lcd.print(receivedData1);
            Serial.println("Received: " + receivedData1);
            receivedData1 = "";
        } else {
            lcd.print(receivedData2);
            Serial.println("Received: " + receivedData2);
            receivedData2 = "";
        }
    }
