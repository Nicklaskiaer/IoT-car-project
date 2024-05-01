#include "esp-now-manager.h"
#include "mqtt-manager.h"
#include "lora-manager.h"
#include "udp-manager.h"
#include "helpers.h"

char modeArray[4] = {'E', 'L', 'M', 'U'};
char currentMode = modeArray[0];

void setup()
{
    WiFi.disconnect();
    Serial.begin(9600);
    initModeSelector();
    initLCD();
    Serial.print("\nChanged mode to: ");
    Serial.println(currentMode);

    if (currentMode == 'E')
    {
        // Initialize ESP-NOW
        initESPNow();
    }
    else if (currentMode == 'L')
    {
        // Initialize LoRa
        initLoRa();
    }
    else if (currentMode == 'M')
    {
        // Initialize MQTT
        initMQTT();
    }
    else if (currentMode == 'U')
    {
        // Initialize UDP
        initUDP();
    }
    else
    {
        Serial.println("ERROR: mode not recognised.");
    }
}

void loop()
{
    if (digitalRead(BUTTON_PIN) == HIGH)
    {
        Serial.println("Changing mode...");
        currentMode = nextMode(currentMode, modeArray);
        delay(2000);
        // Run the setup function again to reinitialize the selected mode
        setup();
    }

    // Read the values of the X and Y pins
    int valueX = analogRead(VRX_PIN);
    int valueY = analogRead(VRY_PIN);

    // Compute the direction of the joystick
    char direction = getCharDirection(valueX, valueY);

    if (currentMode == 'E')
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
    else if (currentMode == 'L')
    {
        // Send the direction using LoRa
        if (sendDirectionOnLoRa(direction))
        {
            Serial.println("Direction sent successfully");
        }
        else
        {
            Serial.println("Failed to send direction");
        }
    }
    else if (currentMode == 'M')
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
    else if (currentMode == 'U')
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