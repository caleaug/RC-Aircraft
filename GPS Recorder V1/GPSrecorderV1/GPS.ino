GPSData GetData() {
  GPSData d1;
  if (gps.time.isValid()) {
    d1.hour = gps.time.hour();
    d1.min = gps.time.minute();
    d1.sec = gps.time.second();
  } else {
    d1.hour = 0; d1.min = 0; d1.sec = 0;
  }

  d1.lat = gps.location.isValid() ? gps.location.lat() : 0;
  d1.lng = gps.location.isValid() ? gps.location.lng() : 0;
  d1.alt = gps.altitude.isValid() ? gps.altitude.feet() : 0;

  d1.sat = gps.satellites.value();
  d1.acc = gps.hdop.hdop();
  return d1;
}

int lastEncodedSecond = -1;
void LogData() {
  while (true){
    while (SerialGPS.available() > 0) {
      if (gps.encode(SerialGPS.read())) {
        
        // Check if the GPS has a valid time and if the second has changed
        if (gps.time.isValid() && gps.time.second() != lastEncodedSecond) {
          
          lastEncodedSecond = gps.time.second(); // Update the tracker
          
          GPSData d = GetData();
          addRow("/data.csv", d);

          Serial.printf("%02d:%02d:%02d, %.6f, %.6f, %.2f, %d, %.2f\n", 
                  d.hour, d.min, d.sec, 
                  d.lat, d.lng, d.alt, 
                  d.sat, d.acc);
        }
      }
    }
  }
}