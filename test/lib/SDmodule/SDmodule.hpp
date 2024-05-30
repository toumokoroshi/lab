#ifndef SDMODULE_HPP
#define SDMODULE_HPP
#include <Arduino.h>
#include <FS.h>
#include <SPI.h>

bool writeFile(fs::FS &fs, const char *path, const char *message);
bool createFile(fs::FS &fs, const char *path);
#endif