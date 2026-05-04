#include <esp_now.h>
#include <WiFi.h>

// MAC Address of your RECEIVER board
uint8_t broadcastAddress[] = {0x00, 0x4B, 0x12, 0xEC, 0xBC, 0xF0};

typedef struct struct_message {
  int id;
  float temp;
  int status;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

// Callback when data is sent (Optional, helps with debugging)
void OnDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  
  // To access the MAC address now, you would use tx_info->dest_addr
  
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Delivery Success");
  } else {
    Serial.println("Delivery Fail");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  myData.id = 1;
  myData.temp = random(2000, 3000) / 100.0; // Simulated float
  myData.status = 1;

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}