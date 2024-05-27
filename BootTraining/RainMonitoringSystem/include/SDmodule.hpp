#include <Arduino.h>
#include <FS.h>

const int CLK = 19;
const int MISO = 29;
const int MOSI = 23;
const int CS = 4;

void readFile(fs::FS &fs, const char *path);
void writeFile(fs::FS &fs, const char *path, const char *message);
void appendFile(fs::FS &fs, const char *path, const char *message);
void deleteFile(fs::FS &fs, const char *path);
void testFileIO(fs::FS &fs, const char *path);