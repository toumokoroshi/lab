#include <Arduino.h>
#include <FS.h>

// Function to write data to the SD card
bool writeFile(fs::FS &fs, const char *path, const char *message) {
    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        return false;
    }
    if (!file.print(message)) {
        file.close();
        return false;
    }
    file.close();
    return true;
}

// Function to create a file on the SD card
bool createFile(fs::FS &fs, const char *path) {
    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        return false;
    }
    file.close();
    return true;
}


