#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>


/****** WiFi Connection Details *******/
const char* ssid = "AndroidAP1063";
const char* password = "alamakota123";

/******* MQTT Broker Connection Details *******/
const char* mqtt_server = "dfb0ec72cc864eddaee0fe147972f4af.s1.eu.hivemq.cloud";
const char* mqtt_username = "publisher-test";
const char* mqtt_password = "Default1";
const int mqtt_port = 8883;

WiFiClientSecure espClient;

/**** MQTT Client Initialisation Using WiFi Connection *****/
PubSubClient client(espClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (32)
char msg[MSG_BUFFER_SIZE];

static const char *fingerprint PROGMEM = "2F 9F 32 1F 17 F7 7A 82 B7 63 93 63 7F C2 ED E3 7F 81 CE 73";

/************* Connect to WiFi ***********/
void setup_wifi() {
  
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


/************* Connect to MQTT Broker ***********/
void reconnect() {
  
  // Loop until we're reconnected
  while (!client.connected()) {
    
    Serial.print("Attempting MQTT connection...");
    String clientId = "publisher-test";   // Create a random client ID

    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      client.subscribe("car-data");   // subscribe the topics here

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/***** Call back Method for Receiving MQTT messages and Switching LED ****/

void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];

  Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);

  // //--- check the incomming message
  //   if ( strcmp(topic,"led_state") == 0) {
      
  //     if (incommingMessage.equals("1"));   // Turn the LED on
      
  //     else ;  // Turn the LED off
  // }
}


/**** Method for Publishing MQTT Messages **********/
void publishMessage(const char* topic, char * payload , boolean retained){
  if (client.publish(topic, payload, true))
    Serial.println("Message publised ["+String(topic)+"]: "+payload);
}


void setup() {
  // initialize digital pin LED_BUILTIN as an output.

  Serial.begin(9600);
  while (!Serial) delay(10);
  setup_wifi();
  
  espClient.setInsecure();
  // espClient.setFingerprint(fingerprint);      // enable this line and the the "certificate" code for secure connection
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}


/******** Main Function *************/
void loop() {

  if (!client.connected()) 
    reconnect(); // check if client is connected
  
  client.loop();

  const * char message = "Test"; 

  publishMessage("car_control", message, true);

  delay(10000);

}