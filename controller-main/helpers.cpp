#include "helpers.h"

// Joystick variables
char direction; //  to store the direction the joystick is pointed
int valueX = 0; // to store the X-axis value
int valueY = 0; // to store the Y-axis value

// Wifi credentials
const char *ssid = "Nicklas - iPhone";
const char *password = "mki123mki";

/*
Function that computes the direction the joystick is pointed to.
Returns a char representing the direction:
f = forwards
b = backwards
r = right
l = left
x = nowhere/resting
*/
char getCharDirection(int x, int y)
{
    if (x > 2000 && (y < 3000 && y > 1000))
    {
        return 'f';
    }
    if (x < 1000 && (y < 3000 && y > 1000))
    {
        return 'b';
    }
    if (y < 1100 && (x < 2000 && x > 1000))
    {
        return 'r';
    }
    if (y > 2500 && (x < 2000 && x > 1000))
    {
        return 'l';
    }
    return 'x';
}

/*
Function that setups the wifi connection for MQTT.
Connects the esp to the defined wifi.
*/
void setupWifi()
{
    delay(10);
    Serial.print("\nConnecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());
    Serial.println("\nWiFi connected\nIP address: ");
    Serial.println(WiFi.localIP());
}

bool changeProtocol();
bool displayProtocol();