void addRow(const char* path, GPSData d) {
    File file = LittleFS.open(path, FILE_APPEND);
    if(!file) {
        Serial.println("- failed to open file for appending");
        return;
    }
    file.printf("%02d:%02d:%02d,%.6f,%.6f,%.2f,%d,%.2f\n", 
                d.hour, d.min, d.sec, d.lat, d.lng, d.alt, d.sat, d.acc);
    file.close();
}

void printCSV(const char* path) {
  Serial.printf("Reading file: %s\n", path);

  File file = LittleFS.open(path, FILE_READ);

  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("--- Start of File ---");

  while (file.available()) {
    Serial.write(file.read());
  }

  Serial.println("\n--- End of File ---");

  file.close(); 
}

void clearCSV(const char* path) {
  File file = LittleFS.open(path, FILE_WRITE);
  
  if (!file) {
    Serial.printf("Failed to open %s for clearing\n", path);
    return;
  }

  file.close();
  Serial.printf("File %s cleared successfully.\n", path);
}
