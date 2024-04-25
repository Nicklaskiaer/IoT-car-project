#include "esp-now-manager.h"

uint8_t broadcastAddress[] = {0x24, 0x0A, 0xC4, 0x80, 0x4C, 0xBC};
esp_now_peer_info_t peerInfo;
bool peerAdded = false; // Keep track if peer is already added

/*
Function that sends the direction to another esp defined by the global broadcastAddress variable.
Returns true if successful otherwise returns false.
*/
bool sendDirectionOnESPNow(char direction)
{
    if (!peerAdded)
    {
        // Register peer only once
        memcpy(peerInfo.peer_addr, broadcastAddress, 6);
        peerInfo.channel = 0;
        peerInfo.encrypt = false;
        if (esp_now_add_peer(&peerInfo) != ESP_OK)
        {
            Serial.println("Failed to add peer");
            return false;
        }
        peerAdded = true;
    }

    // Send the direction data
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&direction, sizeof(direction));
    if (result == ESP_OK)
    {
        Serial.println("Sent with success");
        Serial.println(direction);

        return true;
    }
    else
    {
        Serial.println("Error sending the data");
        return false;
    }
}
bool initESPNow()
{
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return false;
    }
    return true;
}