void addRow(const char* path, data d) {
  File file = LittleFS.open(path, FILE_APPEND);
  
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }

  // Formatting the string:
  // %02d ensures the time has leading zeros (e.g., 09:05:01)
  // %f handles the doubles and floats
  file.printf("%02d:%02d:%02d, %.6f, %.6f, %.2f, %d, %.2f\n", 
              d.hour, d.min, d.sec, 
              d.lat, d.lng, d.alt, 
              d.sat, d.acc);

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
  File file = LittleFS.open(path, FILE_WRITE);
  
  if (!file) {
    Serial.printf("Failed to open %s for clearing\n", path);
    return;
  }

  file.close();
  Serial.printf("File %s cleared successfully.\n", path);
}
