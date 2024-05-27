#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

extern const int CLK;
extern const int MISO;
extern const int MOSI;
extern const int CS;

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(10);
    }

    SPI.begin(CLK, MISO, MOSI, CS);

    writeFile(SD, "/hello.txt", "Hello ");
    appendFile(SD, "/hello.txt", "World!\n");
    readFile(SD, "/hello.txt");
    deleteFile(SD, "/foo.txt");
    renameFile(SD, "/hello.txt", "/foo.txt");
    readFile(SD, "/foo.txt");
    testFileIO(SD, "/test.txt");
    Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void loop() {}