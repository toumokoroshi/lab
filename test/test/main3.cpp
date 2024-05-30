

// receiving data format : X,XX,XYY,XYY,XXXY,XXXY,X,XXXX,ZXXY,XXY\r\n

// label            			index	format  unit 	digits(DEC)
// Transmit Counter				1		X	    -	    1
// AnomaryID					2		XX	    -	    2
// solar panel voltage value	3		X.YY	V	    3
// battery Voltage Value		4		X.YY	V	    3
// water detector voltage value	5		X.YYY	V       4
// current value				6		XXX.Y	mA	    4
// precipitation				7		XXX.Y   mm      4
// atomospheric preassure		8		XXXX	hPa	    4
// temperature					9		ZXX.Y	℃	   4  Z = 1の時プラス
// humidity						10		XX.Y	%	    3
//                                                      total : 43digits


/*
           3V3 - |o3V3      GNDo| -
               - |oEN        23o| - MOSI
               - |oVP        22o| -
               - |oVN        TXo| - TX
               - |o34        RXo| - RX
               - |o35        21o| -
               - |o32       GNDo| -
            EN - |o33        19o| - MISO
               - |o25        18o| - CLK
               - |o26         5o| -
            RS - |o27        17o| - DB7
           DB4 - |o14        16o| - DB6
           LED - |o12         4o| - CS
           GND - |oGND        0o| -
    DATASWITCH - |o13         2o| -
               - |oD2        15o| - DB5
               - |oD3        D1o| -
               - |oCMD       D0o| -
               - |o5V       CLKo| -
*/

#include <Arduino.h>
#include <stdlib.h>
#include <string.h>
#include <LiquidCrystal.h>
// #include <SDmodule.hpp>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <setup.hpp>
LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7);

// void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
//   Serial.printf("Listing directory: %s\n", dirname);

//   File root = fs.open(dirname);
//   if (!root) {
//     lcd.print("Failed to open directory");
//     return;
//   }
//   if (!root.isDirectory()) {
//     lcd.print("Not a directory");
//     return;
//   }

//   File file = root.openNextFile();
//   while (file) {
//     if (file.isDirectory()) {
//       Serial.print("  DIR : ");
//       lcd.print(file.name());
//       if (levels) {
//         listDir(fs, file.path(), levels - 1);
//       }
//     } else {
//       Serial.print("  FILE: ");
//       Serial.print(file.name());
//       Serial.print("  SIZE: ");
//       lcd.print(file.size());
//     }
//     file = root.openNextFile();
//   }
// }

void createDir(fs::FS &fs, const char *path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    lcd.print("Dir created");
  } else {
    lcd.print("mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char *path) {
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path)) {
    lcd.print("Dir removed");
  } else {
    lcd.print("rmdir failed");
  }
}

void readFile(fs::FS &fs, const char *path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    lcd.print("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    lcd.print("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    lcd.print("File written");
  } else {
    lcd.print("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    lcd.print("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    lcd.print("Message appended");
  } else {
    lcd.print("Append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char *path1, const char *path2) {
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    lcd.print("File renamed");
  } else {
    lcd.print("Rename failed");
  }
}

void deleteFile(fs::FS &fs, const char *path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    lcd.print("File deleted");
  } else {
    lcd.print("Delete failed");
  }
}

// void testFileIO(fs::FS &fs, const char *path) {
//   File file = fs.open(path);
//   static uint8_t buf[512];
//   size_t len = 0;
//   uint32_t start = millis();
//   uint32_t end = start;
//   if (file) {
//     len = file.size();
//     size_t flen = len;
//     start = millis();
//     while (len) {
//       size_t toRead = len;
//       if (toRead > 512) {
//         toRead = 512;
//       }
//       file.read(buf, toRead);
//       len -= toRead;
//     }
//     end = millis() - start;
//     Serial.printf("%u bytes read for %lu ms\n", flen, end);
//     file.close();
//   } else {
//     lcd.print("Failed to open file for reading");
//   }

//   file = fs.open(path, FILE_WRITE);
//   if (!file) {
//     lcd.print("Failed to open file for writing");
//     return;
//   }

//   size_t i;
//   start = millis();
//   for (i = 0; i < 2048; i++) {
//     file.write(buf, 512);
//   }
//   end = millis() - start;
//   Serial.printf("%u bytes written for %lu ms\n", 2048 * 512, end);
//   file.close();
// }
                                                                                                
void setup() {
    // Serial.begin(115200);
    lcd.begin(16, 2);
    lcd.print("Initializing...");
    SD.begin();
    delay(1000);
  if (!SD.begin()) {
    lcd.autoscroll();
    lcd.print("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    lcd.print("No SD card attached");
    return;
  }

  lcd.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    lcd.print("MMC");
  } else if (cardType == CARD_SD) {
    lcd.print("SDSC");
  } else if (cardType == CARD_SDHC) {
    lcd.print("SDHC");
  } else {
    lcd.print("UNKNOWN");
  }


//   listDir(SD, "/", 0);
//   createDir(SD, "/mydir");
//   listDir(SD, "/", 0);
//   removeDir(SD, "/mydir");
//   listDir(SD, "/", 2);
//   writeFile(SD, "/hello.txt", "Hello ");
//   appendFile(SD, "/hello.txt", "World!\n");
//   readFile(SD, "/hello.txt");
//   deleteFile(SD, "/foo.txt");
//   renameFile(SD, "/hello.txt", "/foo.txt");
//   readFile(SD, "/foo.txt");
//   testFileIO(SD, "/test.txt");
}

void loop() {}
