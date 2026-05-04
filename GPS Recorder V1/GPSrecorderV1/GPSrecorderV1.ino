#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include "LittleFS.h"

struct GPSData {
  int hour;
  int min;
  int sec;
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

void loop() {
  String out = READ();

  if(out.length() > 0){
    if(out.substring(0,5).equals("clear")){clearCSV("/data.csv");}
    if(out.substring(0,3).equals("log")){LogData();}
    if(out.substring(0,5).equals("print")){printCSV("/data.csv");}
  }
}

String READ() {
  String out = "";
  
  if (Serial.available() > 0) {
    delay(10); 
    
    while (Serial.available() > 0) {
      char c = Serial.read(); 
      out += c;
    }
    //Serial.println(out);
  }
  return out;
}
