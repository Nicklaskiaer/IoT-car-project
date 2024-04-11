#include <LoRa.h>  // LoRa by Sandeep Mistry
#include <WiFi.h>  // WiFiManager by tzapu
#include <esp_now.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

/**
The MODE variable is used to dictate what protocol the controller should send the direction data with.

Supports:
ESP_NOW
LORA
MQTT
UDP

*/
#define MODE "MQTT"

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

// Other definitions
#define MSG_BUFFER_SIZE (8)

// Joystick variables
char direction; //  to store the direction the joystick is pointed
int valueX = 0; // to store the X-axis value
int valueY = 0; // to store the Y-axis value

// ESP variables
uint8_t broadcastAddress[] = {0x24, 0x0A, 0xC4, 0x80, 0x4C, 0xBC};
esp_now_peer_info_t peerInfo;
bool peerAdded = false; // Keep track if peer is already added

// MQTT variables
const char* ssid = "AndroidAP1063";
const char* password = "alamakota123";
const char* mqtt_server = "dfb0ec72cc864eddaee0fe147972f4af.s1.eu.hivemq.cloud";
const char* mqtt_username = "controller-test";
const char* mqtt_password = "ControllerTest1";
const int mqtt_port = 8883;
unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];
WiFiClientSecure espClient;
PubSubClient client(espClient);
static const char *rootCA PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";


// Function prototypes
char getCharDirection(int x, int y);
// ESP_NOW
bool sendDirectionOnESPNow(char direction);
// LORA
bool sendDirectionOnLora(char direction);
// MQTT
void setup_wifi();
void reconnectWithMQTTBroker();
void MQTTcallback(char* topic, byte* payload, unsigned int length);
void MQTTpublishMessage(const char* topic, const char * payload , boolean retained);
// UDP


void setup() {
  Serial.begin(9600);
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);

  if (MODE == "ESP_NOW") {
      // Initialize ESP-NOW
      WiFi.mode(WIFI_STA);
      if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
      }
  } else if (MODE == "LORA"){
      // Initialize LoRa
      LoRa.setPins(SS, RST, DI0);
      if (!LoRa.begin(915E6)) {
        Serial.println("Starting LoRa failed!");
        while (1);
      }
  } else if (MODE == "MQTT"){
      // Initialize MQTT
      MQTTsetup_wifi();
      // If we do not want to use the rootCA, uncommet the line underneath. 
      // espClient.setInsecure();
      espClient.setCACert(rootCA);
      client.setServer(mqtt_server, mqtt_port);
      client.setCallback(MQTTcallback);
  } else if (MODE == "UDP") {
      // Initialize UDP
  }
}

/*
Function that computes the direction the joystick is pointed to. 
Returns a char representing the direction:
f = forwards
b = backwards
r = right
l = left
x = nowhere/resting
*/
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

/*
Function that setups the wifi connection for MQTT.
Connects the esp to the defined wifi.
*/
void MQTTsetup_wifi() {
  
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
}

/*
Function to connect to the MQTT broker if the connection is lost.
*/
void MQTTreconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    
    Serial.print("Attempting MQTT connection...");
    String clientId = "publisher-test";   // Create a random client ID

    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

    client.subscribe("time/ack", 0);

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/*
Callback function for the MQTT response messages. 
*/
void MQTTcallback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];

  Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);
}

/*
Function that publishes the message passed to the client.
*/
void MQTTpublishMessage(const char* topic, const char * payload , boolean retained){
  if (client.publish(topic, payload, true))
    Serial.println("Message publised ["+String(topic)+"]: "+payload);
}


void loop() {

  // Read the values of the X and Y pins
  valueX = analogRead(VRX_PIN);
  valueY = analogRead(VRY_PIN);

  // Compute the direction of the joystick
  direction = getCharDirection(valueX, valueY);

  if (MODE == "ESP_NOW") {
      // send the direction using esp_now
      if (sendDirectionOnESPNow(direction)) {
        Serial.println("Direction sent successfully");
      } else {
        Serial.println("Failed to send direction");
      }
  } else if (MODE == "LORA") {
      // send the direction using lora
      if (sendDirectionOnLora(direction)) {
        Serial.println("Direction sent successfully");
      } else {
        Serial.println("Failed to send direction");
      }
  } else if (MODE == "MQTT") {
        // Send the direction using MQTT
        if (!client.connected()) { // check if client is connected
          MQTTreconnect();
        } 
        client.loop();
        MQTTpublishMessage("car_data", &direction, true);
  } else if (MODE == "UDP") {
      // Initialize UDP
  }
  delay(100);

}