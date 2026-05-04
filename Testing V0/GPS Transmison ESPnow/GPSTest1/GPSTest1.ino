#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include "LittleFS.h"

struct data{
  double lat;
  double lng;
  double alt;
  int sat;
  float acc;
};

TinyGPSPlus gps;
HardwareSerial SerialGPS(2); // Use HardwareSerial 2 for ESP32 (pins 16 and 17 by default)

void setup() {
  Serial.begin(115200);
  
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17); 
  Serial.println("ESP32 GPS Initializing...");

  if(!LittleFS.begin(true)){
    Serial.println("LittleFS Mount Failed");
    return;
  }
}

void loop(){
  clearCSV("/data.csv");
  loop2();
}


void loop2(){
  printCSV("/data.csv");
  delay(10000000);
}

void loop1() {
  while (SerialGPS.available() > 0) {
    if (gps.encode(SerialGPS.read())) {
      data d1;
      d1 = GetData();
      PrintData(d1);
      addRow("18:18", d1.acc);
    }
  }
  
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println("Error: No GPS detected. Check wiring.");
    while(true);
  }
}

data GetData(){
  data d1;
  if (gps.location.isValid()) {d1.lat = gps.location.lat();}
  else {d1.lat = 0;}
  if (gps.location.isValid()) {d1.lng = gps.location.lng();}
  else {d1.lng = 0;}
  if (gps.altitude.isValid()) {d1.alt = gps.altitude.feet();}
  else {d1.alt = 0;}

  d1.sat = gps.satellites.value();
  d1.acc = gps.hdop.hdop();
  return d1;
}

void PrintData(const data& d1){
  Serial.printf("Lat: %.6f, Lng: %.6f, Alt: %.2f ft, Sat: %d, Acc: %.2f\n",
  d1.lat,d1.lng,d1.alt,d1.sat,d1.acc);
}

void addRow(String timestamp, float temp) {
  File file = LittleFS.open("/data.csv", FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  // Standard CSV format: Column1,Column2\n
  file.printf("%s,%.2f\n", timestamp.c_str(), temp);
  file.close();
}

void printCSV(const char* path) {
  Serial.printf("Reading file: %s\n", path);

  // Open the file for reading ("r")
  File file = LittleFS.open(path, FILE_READ);

  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("--- Start of File ---");

  // Read from the file until there's nothing left
  while (file.available()) {
    // Read and print each character to the Serial monitor
    Serial.write(file.read());
  }

  Serial.println("\n--- End of File ---");

  file.close(); 
}

void clearCSV(const char* path) {
  // Opening with FILE_WRITE ("w") deletes existing content
  File file = LittleFS.open(path, FILE_WRITE);
  
  if (!file) {
    Serial.printf("Failed to open %s for clearing\n", path);
    return;
  }

  file.close();
  Serial.printf("File %s cleared successfully.\n", path);
}

