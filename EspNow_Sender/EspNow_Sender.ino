#include <WiFi.h>
#include <esp_now.h>
 
uint8_t broadcastAddress[] = {0x24, 0x62, 0xAB, 0xCA, 0xCA, 0xCC};

esp_now_peer_info_t peerInfo;

void setup(){
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop(){
  esp_err_t result = esp_now_send(broadcastAddress);
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}
