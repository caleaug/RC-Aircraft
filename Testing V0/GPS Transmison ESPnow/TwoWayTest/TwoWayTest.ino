#include <esp_now.h>
#include <WiFi.h>

// 1. CHANGE THIS for each board to the OTHER board's MAC
//uint8_t broadcastAddress[] = {0xCC, 0xDB, 0xA7, 0x9F, 0x7E, 0x60}; //CC:DB:A7:9F:7E:60
uint8_t broadcastAddress[] = {0x00, 0x4B, 0x12, 0xEC, 0xBC, 0xF0}; //00:4B:12:EC:BC:F0

typedef struct struct_message {
  int id;
  float val;
} struct_message;

struct_message myData;      // Data to send
struct_message incomingData; // Data received
esp_now_peer_info_t peerInfo;

// Callback for SENDING (V3.0+ Signature)
void OnDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  // Serial.print("Send Status: ");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

// Callback for RECEIVING (V3.0+ Signature)
void OnDataRecv(const esp_now_recv_info_t * recv_info, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));
  
  // Format for Python: ID,Value
  Serial.print(incomingData.id);
  Serial.print(",");
  Serial.println(incomingData.val);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    return;
  }

  // Register both callbacks
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    return;
  }
}

void loop() {
  // Example: Send an incrementing value every 5 seconds
  myData.id = 2; // Unique ID for this board
  myData.val = millis() / 1000.0; 

  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  delay(5000); 
}