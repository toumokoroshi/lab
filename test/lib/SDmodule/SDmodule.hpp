#include <Arduino.h>
#include <FS.h>
#include <SPI.h>

void writeFile(fs::FS &fs, const char *path, const char *message);
