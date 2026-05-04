data GetData(){
  data d1;
  if (gps.time.isValid()) {
    d1.hour = gps.time.hour();
    d1.min = gps.time.minute();
    d1.sec = gps.time.second();
  }

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

void LogData(){
  while(true){
    while (SerialGPS.available() > 0) {
      if (gps.encode(SerialGPS.read())) {
        data d;
        d = GetData();
        PrintData(d);
        addRow("/data.csv", d);

        serial.printf("%02d:%02d:%02d, %.6f, %.6f, %.2f, %d, %.2f\n", 
                d.hour, d.min, d.sec, 
                d.lat, d.lng, d.alt, 
                d.sat, d.acc);
      }
    }
  }
}