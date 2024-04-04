#include <WiFi.h>
#include <esp_now.h>

#define VRX_PIN  34
#define VRY_PIN  35

uint8_t broadcastAddress[] = {0x24, 0x62, 0xAB, 0xCA, 0xCA, 0xCC};
esp_now_peer_info_t peerInfo;
int valueX = 0; // to store the X-axis value
int valueY = 0; // to store the Y-axis value
char direction;

char getCharDirection(int x, int y){
  if (x > 2000 && (y < 3000 && y > 1000)){
    return 'f';
  }
  if (x < 1000 && (y < 3000 && y > 1000)){
    return 'b';
  }
  if (y < 1100 && (x < 2000 && x > 1000)){
    return 'r';
  }
  if (y > 2500 && (x < 2000 && x > 1000)){
    return 'l';
  }
  return 'x';
}

bool sendDirectionOnESPNow(char direction){

}


void setup() {
  Serial.begin(9600);

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

void loop() {
  // read X and Y analog values
  valueX = analogRead(VRX_PIN);
  valueY = analogRead(VRY_PIN);

  Serial.print(valueX);
  Serial.print(valueY);

  direction = getCharDirection(valueX, valueY);

  Serial.println(direction);


  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &direction, 1);
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}
