#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <HardwareSerial.h>

#define R "72"
#define F "66"
#define L "6C"
#define B "62"
#define X "78"


// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6)
const int potPin = 34;
const int vout = 21;
// variable for storing the potentiometer value
int potValue = 0;
// 1 = ESPNOW
// 2 = MQTT
int mode = 0;
int newmode = 0;
//-----------------------------------ESP NOW-----------------------------------------
LiquidCrystal_I2C lcd(0x27, 16, 2);
uint8_t data;

HardwareSerial loraSerial(2); // Enable UART2

void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {
  memcpy(&data, incomingData, sizeof(data));
  //Serial.print("Bytes received: ");
  //Serial.println(len);
  //Serial.print("Int: ");
  //Serial.println((char) data);
  Serial.print((char)data);
}

void lora_autobaud()
{
  String response = "";
  while (response=="")
  {
    delay(1000);
    loraSerial.write((byte)0x00);
    loraSerial.write(0x55);
    loraSerial.println();
    loraSerial.println("sys get ver");
    response = loraSerial.readStringUntil('\n');
  }
}

void loraSetup() {
  pinMode(23, OUTPUT);

  loraSerial.begin(57600);  // Serial communication to RN2483, default pins for UART2 RX->16 TX->17
  loraSerial.setTimeout(1000);
  lora_autobaud();
  
  digitalWrite(23, LOW);
  delay(300);
  digitalWrite(23, HIGH);

  Serial.println("Initing LoRa");
  
  String str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  loraSerial.println("sys get ver");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("mac pause");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
//  loraSerial.println("radio set bt 0.5");
//  wait_for_ok();
  
  loraSerial.println("radio set mod lora");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set freq 868000000");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set pwr 14");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set sf sf7");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set afcbw 41.7");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set rxbw 20.8");  // Receiver bandwidth can be adjusted here. Lower BW equals better link budget / SNR (less noise). 
  str = loraSerial.readStringUntil('\n');   // However, the system becomes more sensitive to frequency drift (due to temp) and PPM crystal inaccuracy. 
  Serial.println(str);
  
//  loraSerial.println("radio set bitrate 50000");
//  wait_for_ok();
  
//  loraSerial.println("radio set fdev 25000");
//  wait_for_ok();
  
  loraSerial.println("radio set prlen 8");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set crc on");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set iqi off");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set cr 4/5"); // Maximum reliability is 4/8 ~ overhead ratio of 2.0
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set wdt 60000"); //disable for continuous reception
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set sync 12");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  
  loraSerial.println("radio set bw 125");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
}

void loraLoop() {
  loraSerial.println("radio rx 0"); //wait for 60 seconds to receive
  String str = loraSerial.readStringUntil('\n');
  delay(20);
  if ( str.indexOf("ok") == 0 )
  {
    String str_data = String("");
    // This is blocking call
    while(str_data=="")
    {
      str_data = loraSerial.readStringUntil('\n');
    }
    
    if ( str_data.indexOf("radio_rx") == 0 )  //checking if data was reeived (equals radio_rx = <data>). indexOf returns position of "radio_rx"
    { 
      if (str_data.indexOf(R) != -1) {
        Serial.print("r");
      }

      if (str_data.indexOf(L) != -1) {
        Serial.print("l");
      }

      if (str_data.indexOf(F) != -1) {
        Serial.print("f");
      }

      if (str_data.indexOf(B) != -1) {
        Serial.print("b");
      }
    }
  }
  else
  {
    delay(2000);
  }
}

void espnowsetup() {
  Serial.println("espnowsetup");
  // lcd.begin();
  delay(500);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello");
  lcd.setCursor(0, 1);
  lcd.print("World");
  lcd.display();

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

void espnowloop() {
  Serial.println("espnowloop");
}

//------------------------------------MQTT-------------------------------------------

/****** WiFi Connection Details *******/
const char* ssid = "AndroidAP1063";
const char* password = "alamakota123";

/******* MQTT Broker Connection Details *******/
const char* mqtt_server = "dfb0ec72cc864eddaee0fe147972f4af.s1.eu.hivemq.cloud";
const char* mqtt_username = "auto-car";
const char* mqtt_password = "420Driving420";
const int mqtt_port = 8883;

const char* data_topic = "car_data";
const char* latency_topic = "time/ack";

WiFiClientSecure espClient;

/**** MQTT Client Initialisation Using WiFi Connection *****/
PubSubClient client(espClient);

static const char* rootCA PROGMEM = R"EOF(
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
    String clientId = "car-test";

    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(data_topic, 0);

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");  // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/***** Call back Method for Receiving MQTT messages****/
void callback(char* topic, byte* payload, unsigned int length) {

  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage += (char)payload[i];
  //Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);
  Serial.print((char)payload[0]);
  client.publish(latency_topic, "ack");
}

void mqttsetup() {
  Serial.println("mqttsetup");
  while (!Serial) delay(10);
  setup_wifi();

  // espClient.setInsecure();
  espClient.setCACert(rootCA);
  client.setBufferSize(512);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void mqttloop() {
  Serial.println("mqttloop");
  if (!client.connected())
    reconnect();  // check if client is connected

  client.loop();
  delay(10);
}

//------------------------------------YEAH-------------------------------------------

void setup() {
  pinMode(vout, OUTPUT);
  digitalWrite(vout, HIGH);
  Serial.begin(9600);
  loraSetup();
}

void loop() {
  potValue = analogRead(potPin);
  int newmode = map(potValue, 0, 4095, 1, 5);

  // if (mode != newmode) {
  //   mode = newmode;
  //   if (mode == 1) {
  //     espnowsetup();
  //   }
  //   if (mode == 2) {
  //     mqttsetup();
  //   }
  //   if (mode == 3) {
  //     loraSetup();
  //   }
  // }

  // if (mode == 1) {
  //   espnowloop();
  // }
  // if (mode == 2) {
  //   mqttloop();
  // }
  // if (mode == 3) {
  //   loraLoop();
  // }

  loraLoop();

  delay(10);
}