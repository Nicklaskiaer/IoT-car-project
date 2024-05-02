#include "mqtt-manager.h"

// Change to match the broker, servername blocked for public repo.
const char *mqtt_server = "hiddenhiddenhidden.s1.eu.hivemq.cloud";
const char *mqtt_username = "controller-test";
const char *mqtt_password = "ControllerTest1";
const int mqtt_port = 8883;
unsigned long lastMsg = 0;
char msg[MSG_BUFFER_SIZE];
WiFiClientSecure espClient;
PubSubClient client(espClient);

/*
Function to connect to the MQTT broker if the connection is lost.
*/
void MQTTreconnect()
{

    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        String clientId = "publisher-test"; // Create a random client ID

        // Attempt to connect
        if (client.connect(clientId.c_str(), mqtt_username, mqtt_password))
        {
            Serial.println("connected");

            client.subscribe("time/ack", 0);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds"); // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

/*
Callback function for the MQTT response messages.
*/
void MQTTcallback(char *topic, byte *payload, unsigned int length)
{
    String incommingMessage = "";
    for (int i = 0; i < length; i++)
        incommingMessage += (char)payload[i];

    Serial.println("Message arrived [" + String(topic) + "]" + incommingMessage);
}

/*
Function that publishes the message passed to the client.
*/
void MQTTpublishMessage(const char *topic, const char *payload, boolean retained)
{
    if (client.publish(topic, payload, true))
        Serial.println("Message publised [" + String(topic) + "]: " + payload);
}

/*
Function that sends the direction over MQTT.
*/
bool sendDirectionOnMQTT(char direction)
{
    if (!client.connected())
    {
        MQTTreconnect();
    }
    MQTTpublishMessage("car_data", String(direction).c_str(), true);
    return true;
}

/*
Function that initializes the MQTT connection.
*/
bool initMQTT()
{
    updateScreen('M');
    setupWifi();

    // If we do not want to use the rootCA, uncommet the line underneath.
    espClient.setInsecure();
    // espClient.setCACert(rootCA);
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(MQTTcallback);
    return true;
}
