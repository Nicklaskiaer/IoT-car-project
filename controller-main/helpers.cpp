#include "helpers.h"

// Joystick variables
char direction; //  to store the direction the joystick is pointed
int valueX = 0; // to store the X-axis value
int valueY = 0; // to store the Y-axis value

// Wifi credentials
const char *ssid = "POCO X3 NFC";
const char *password = "Kodeord1234";
// const char *ssid = "Nicklas - iPhone";
// const char *password = "mki123mki";

// Screen variables
LiquidCrystal_I2C lcd(0x27, 16, 2);

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

bool initModeSelector()
{
    // Set the button pin as input
    pinMode(BUTTON_PIN, INPUT);
    return true;
}

bool buttonPushed()
{
    if (true)
    {
        return true;
    }
    return false;
}

/*
Function that changes the current mode to the next mode in the mode array
Returns a pointer to the new node.
*/
char nextMode(char currentMode, char *modeArray)
{
    int i = 0;
    while (modeArray[i] != currentMode)
    {
        i++;
    }
    if (i == 4)
    {
        return modeArray[0];
    }
    else
    {
        return modeArray[i +1];
    }
}

bool initLCD(){
  lcd.begin(0x27, 16, 2);
  delay(250);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing");
  lcd.setCursor(0, 1);
  lcd.print(".");
  lcd.display();
  return true;
}

/*
Function that updates the LCD screen for the controller to fit with the currently used protocol.
*/
bool updateScreen(char currentMode){
  if (currentMode == 'E') {
      lcd.clear();
      delay(50);
      lcd.setCursor(0, 0);
      lcd.print("Running");
      lcd.setCursor(0, 1);
      lcd.print("ESP NOW");
      lcd.display();
    }
    if (currentMode == 'L') {
      lcd.clear();
      delay(50);
      lcd.setCursor(0, 0);
      lcd.print("Running");
      lcd.setCursor(0, 1);
      lcd.print("LoRa");
      lcd.display();
    }
    if (currentMode == 'M') {
      lcd.clear();
      delay(50);
      lcd.setCursor(0, 0);
      lcd.print("Running");
      lcd.setCursor(0, 1);
      lcd.print("MQTT");
      lcd.display();
    }
    if (currentMode == 'U') {
      lcd.clear();
      delay(50);
      lcd.setCursor(0, 0);
      lcd.print("Running");
      lcd.setCursor(0, 1);
      lcd.print("UDP");
      lcd.display();
    }
    if (currentMode == 'B') {
      lcd.clear();
      delay(50);
      lcd.setCursor(0, 0);
      lcd.print("Running");
      lcd.setCursor(0, 1);
      lcd.print("BLE");
      lcd.display();
    }
    return true;
  }
