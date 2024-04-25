#include "esp-now-manager.h"
#include "mqtt-manager.h"
#include "lora-manager.h"
#include "udp-manager.h"
#include "helpers.h"

char modeArray[5] = {'E', 'L', 'M', 'U', 'B'};
char currentMode = modeArray[0];

void setup()
{
    WiFi.disconnect();
    loraSerial.
    Serial.begin(9600);

    initModeSelector();
    Serial.print("\nNew mode selected: ");
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
        // initUDP();
    }
    else if (currentMode == 'B')
    {
        // Initialize BLE
        // initBLE();
    }
    else
    {
        Serial.print("ERROR: mode not recognised.");
    }
}

void loop()
{
    if (digitalRead(BUTTON_PIN) == HIGH)
    {
        Serial.print("Button pushed");
        currentMode = nextMode(currentMode, modeArray);
        Serial.print("Mode changed to: ");
        Serial.println(currentMode);
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
        // if (sendDirectionOnUDP(direction))
        // {
        //     Serial.println("Direction sent successfully");
        // }
        // else
        // {
        //     Serial.println("Failed to send direction");
        // }
    }
    else if (currentMode == 'B')
    {
        // Send the direction using BLE
        // if (sendDirectionOnUDP(direction))
        // {
        //     Serial.println("Direction sent successfully");
        // }
        // else
        // {
        //     Serial.println("Failed to send direction");
        // }
    }
    delay(100);
}