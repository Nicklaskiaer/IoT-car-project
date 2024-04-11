#include <LoRa.h>  // LoRa by Sandeep Mistry
#include <WiFi.h>  // WiFiManager by tzapu
#include <esp_now.h>

/**
The MODE variable is used to dictate what protocol the controller should send the direction data with.

Supports:
ESP_NOW
LORA
MQTT
UDP

*/
#define MODE "ESP_NOW"

// Pin definitions
#define VRX_PIN   34  // GPIO34 -- X direction
#define VRY_PIN   35  // GPIO35 -- Y direction
#define POWER_PIN 23  // GPIO23 -- Second 3V3 output
#define SCK       5   // GPIO5 -- SX1278's SCK
#define MISO      19  // GPIO19 -- SX1278's MISO
#define MOSI      27  // GPIO27 -- SX1278's MOSI
#define SS        18  // GPIO18 -- SX1278's CS
#define RST       14  // GPIO14 -- SX1278's RESET
#define DI0       26  // GPIO26 -- SX1278's IRQ(Interrupt Request)


// Joystick variables
char direction;  //  to store the direction the joystick is pointed
int valueX = 0; // to store the X-axis value
int valueY = 0; // to store the Y-axis value

// ESP variables
uint8_t broadcastAddress[] = {0x24, 0x0A, 0xC4, 0x80, 0x4C, 0xBC};
esp_now_peer_info_t peerInfo;
bool peerAdded = false; // Keep track if peer is already added


// Function prototypes
char getCharDirection(int x, int y);
bool sendDirectionOnESPNow(char direction);
bool sendDirectionOnLora(char direction);


void setup() {
  Serial.begin(9600);
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);

  switch(MODE) {
    case "ESP_NOW":
      // Initialize ESP-NOW
      WiFi.mode(WIFI_STA);
      if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
      }
    
    case "LORA":
      // Initialize LoRa
      LoRa.setPins(SS, RST, DI0);
      if (!LoRa.begin(915E6)) {
        Serial.println("Starting LoRa failed!");
        while (1);
      }
    
    case "MQTT":
      // Initialize MQTT
      break;
    
    case "UDP":
      // Initialize UDP
      break;

    default:
      Serial.println("Mode not recognised")
  }
}


/**
* Function that computes the direction the joystick is pointed to. 
* Returns a char representing the direction:
* f = forwards
* b = backwards
* r = right
* l = left
* x = nowhere/resting
**/
char getCharDirection(int x, int y) {
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

/*
Function that sends the direction to another esp defined by the global broadcastAddress variable.
Returns true if successful otherwise returns false.
*/
bool sendDirectionOnESPNow(char direction){
  if (!peerAdded) {
    // Register peer only once
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
      Serial.println("Failed to add peer");
      return false;
    }
    peerAdded = true;
  }

  // Send the direction data
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &direction, sizeof(direction));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
    Serial.println(direction);

    return true;
  } else {
    Serial.println("Error sending the data");
    return false;
  }
}

/*
Funciton that sends the direction using LoRa.
Returns true if successful otherwise returns false.
*/
bool sendDirectionOnLora(char direction) {
  LoRa.beginPacket();
  LoRa.print(direction);
  int result = LoRa.endPacket();
  
  if (result == 1) {
    Serial.println("Sent over LoRa: ");
    Serial.println(direction);
    return true;
  } else {
    Serial.println("LoRa send failed");
    return false;
  }
}


void loop() {

  // Read the values of the X and Y pins
  valueX = analogRead(VRX_PIN);
  valueY = analogRead(VRY_PIN);

  // Compute the direction of the joystick
  direction = getCharDirection(valueX, valueY);

  switch(MODE) {
    case "ESP_NOW":
      // send the direction using esp_now
      if (sendDirectionOnESPNow(direction)) {
        Serial.println("Direction sent successfully");
      } else {
        Serial.println("Failed to send direction");
      }

    case "LORA":
      // send the direction using lora
      if (sendDirectionOnLora(direction)) {
        Serial.println("Direction sent successfully");
      } else {
        Serial.println("Failed to send direction");
      }

    case "MQTT":
      // Send the direction using MQTT
      break;
    
    case "UDP":
      // Send the direction using UDP
      break;

    default:
      Serial.print("ERROR - Mode not recognised: ");
      Srial.print(MODE);
      break;

  delay(100);
}
