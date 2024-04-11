/*
void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud rate
}

void loop() {
  Serial.print("1");
  delay(500);
  Serial.print("2");
  delay(500);
  Serial.print("3");
  delay(500);
  Serial.print("4");
  delay(500);
}*/

#include <esp_now.h>
#include <WiFi.h>

uint8_t data;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&data, incomingData, sizeof(data));
  //Serial.print("Bytes received: ");
  //Serial.println(len);
  //Serial.print("Int: ");
  //Serial.println((char) data);
  Serial.print((char) data);
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

}