#include "esp-now-manager.h"
#include "mqtt-manager.h"
#include "lora-manager.h"
#include "udp-manager.h"
#include "helpers.h"

#define MODE "UDP" // ["ESP_NOW", "LORA", "MQTT", "UDP"]

void setup()
{
    Serial.begin(9600);

    if (MODE == "ESP_NOW")
    {
        // Initialize ESP-NOW
        initESPNow();
    }
    else if (MODE == "LORA")
    {
        // Initialize LoRa
    }
    else if (MODE == "MQTT")
    {
        // Initialize MQTT
        initMQTT();
    }
    else if (MODE == "UDP")
    {
        // Initialize UDP
        initUDP();
        Serial.print("INIT has been run");
    }
    else
    {
        Serial.print("ERROR: Mode not recognised.");
    }
}

void loop()
{

    Serial.print("In loop");
    // Read the values of the X and Y pins
    int valueX = analogRead(VRX_PIN);
    int valueY = analogRead(VRY_PIN);

    // Compute the direction of the joystick
    char direction = getCharDirection(valueX, valueY);

    if (MODE == "ESP_NOW")
    {
        // send the direction using esp_now
        if (sendDirectionOnESPNow(direction))
        {
            Serial.println("Direction sent successfully");
        }
        else
        {
            Serial.println("Failed to send direction");
        }
    }
    else if (MODE == "LORA")
    {
    }
    else if (MODE == "MQTT")
    {
        // Send the direction using MQTT
        if (sendDirectionOnMQTT(direction))
        {
            Serial.println("Direction sent successfully");
        }
        else
        {
            Serial.println("Failed to send direction");
        }
    }
    else if (MODE == "UDP")
    {
        // Send the direction using UDP
        if (sendDirectionOnUDP(direction))
        {
            Serial.println("Direction sent successfully");
        }
        else
        {
            Serial.println("Failed to send direction");
        }
    }
    delay(100);
}