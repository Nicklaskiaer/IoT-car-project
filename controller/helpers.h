#ifndef HELPERS_H
#define HELPERS_H

#include <WiFi.h>

#define VRX_PIN 34 // GPIO34 -- X direction
#define VRY_PIN 35 // GPIO35 -- Y direction


// Function prototypes
char getCharDirection(int x, int y);
void setupWifi();
bool changeProtocol();
bool displayProtocol();

#endif