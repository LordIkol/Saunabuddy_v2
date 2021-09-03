#include "FS.h"
#include "SD.h"
#include "SPI.h"



void initialize_SD() {
  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  } else {
    Serial.println("Card Mounted");
    return;
  }

  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
  else
  {
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
  }
}

void deleteFile(fs::FS& fs, const char* path) {
    Serial.printf("Deleting file: %s\n", path);
    if (fs.remove(path)) {
        Serial.println("File deleted");
    }
    else {
        Serial.println("Delete failed");
    }
}

void createDir(fs::FS &fs, const char * path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (SD.exists(path)) {
    Serial.println("Dir exists");
    return;
  }
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void readFile(fs::FS &fs, const char * path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);
  if (SD.exists(path)) {
    Serial.println("File Exists");
    return;
  }
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();

}

void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void logToFile(fs::FS &fs, const char* fldr, const char * path, const char * message) {
    int cardType = SD.cardType();
    Serial.println("Card Type:" + cardType);
    if (cardType == CARD_NONE) {
        Serial.println("Card Type:" + cardType);
        return;
    }

    if (!SD.exists(fldr)) {
        createDir(SD, fldr);
    }

    char fullpath[30];
        
    strcpy(fullpath, fldr);
    strcat(fullpath, path);

    if (SD.exists(fullpath)) {
        char startmessage[1024] = ",";
        strcat(startmessage, message);
        appendFile(SD, fullpath, startmessage);
    }
    else {
        char newmessage[1024] = "[";
        strcat(newmessage, message);
        writeFile(SD, fullpath, newmessage);
    }
    
}


//void printlargefile() {
//  readFile(SD, "/yeardata.txt");
//}
