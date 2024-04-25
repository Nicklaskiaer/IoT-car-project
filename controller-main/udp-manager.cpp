#include "udp-manager.h"

WiFiUDP udp;
const char *udpAddress = "172.20.10.3"; // Example IP address of the receiver
const int udpPort = 2390;               // Example UDP port

/*
Function that sends the direction over UDP.
*/
bool sendDirectionOnUDP(char direction)
{
    Serial.print("Sending UDP packet now");
    udp.beginPacket(udpAddress, udpPort);
    udp.write( (uint8_t *)&direction, sizeof(direction));
    if (udp.endPacket() == 1)
    {
        Serial.println("Direction sent over UDP: ");
        Serial.println(direction);
        return true;
    }
    else
    {
        Serial.println("UDP send failed");
        return false;
    }
}

/*
Function to initialize the UDP connection.
*/
bool initUDP()
{
    setupWifi();
    if (udp.begin(udpPort)){
      Serial.print("Successfully began UDP");
    }
    else{
      Serial.print("Error beginning UDP");
    }
}
