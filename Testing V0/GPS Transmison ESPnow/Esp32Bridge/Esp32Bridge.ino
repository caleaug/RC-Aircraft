#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message {
  int id;
  float temp;
  int status;
} struct_message;

struct_message incomingReadings;

void OnDataRecv(const esp_now_recv_info_t * recv_info, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  
  // To get the MAC address now, you'd use recv_info->src_addr
  
  // Format as CSV for Python
  Serial.print(incomingReadings.id);
  Serial.print(",");
  Serial.print(incomingReadings.temp);
  Serial.print(",");
  Serial.println(incomingReadings.status);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Keeping the loop empty; logic is in the callback
}