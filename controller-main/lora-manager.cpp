#include "lora-manager.h"

int RXD2 = 16;
int TXD2 = 17;
int RST = 23;
int packageID = 0;
String str;

HardwareSerial loraSerial(2);

bool initLoRa()
{
  updateScreen('L');
  loraSerial.begin(56700); // This sets the Lora baudrate to 57600 with the
  // correct settings: 8 bit, no flow control and 1 stop bit, and uses UART2
  loraSerial.setTimeout(1000);
  // resets the RN2483 module;
  digitalWrite(RST, LOW);
  delay(200);
  digitalWrite(RST, HIGH);
  // lora_autobaud(); // Automaticly select the correct baudrate by sending a sequence of known data.

  Serial.println("Initing LoRa");

  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  loraSerial.println("sys get ver");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);

  loraSerial.println("mac pause"); // turns of LORAWAN
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);

  //  loraSerial.println("radio set bt 0.5");
  //  wait_for_ok();

  loraSerial.println("radio set mod lora"); // set to LORA mode
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);

  loraSerial.println("radio set freq 868000000"); // sets the selected frequency
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);

  loraSerial.println("radio set pwr 14"); // sets transmision power to the highest
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);

  loraSerial.println("radio set sf sf7"); // sets spreading factor to the lowest
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);

  loraSerial.println("radio set afcbw 41.7"); // auto freq to 41.7
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);

  loraSerial.println("radio set rxbw 20.8"); // Receiver bandwidth can be adjusted here. Lower BW equals better link budget / SNR (less noise).
  str = loraSerial.readStringUntil('\n');    // However, the system becomes more sensitive to frequency drift (due to temp) and PPM crystal inaccuracy.
  Serial.println(str);

  //  loraSerial.println("radio set bitrate 50000");
  //  wait_for_ok();

  //  loraSerial.println("radio set fdev 25000");
  //  wait_for_ok();

  loraSerial.println("radio set prlen 8"); // sets the preamble length which is to synchronization.
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);

  loraSerial.println("radio set crc on"); // activates crc which is used for error detection
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);

  loraSerial.println("radio set iqi off"); // sets IQ invert to off
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);

  loraSerial.println("radio set cr 4/5"); // Maximum reliability is 4/8 ~ overhead ratio of 2.0
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);

  loraSerial.println("radio set wdt 60000"); // it triggers a software reset af 60 sec if there is a system failure.
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);

  loraSerial.println("radio set sync 12"); // Sync word in HEX
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);

  loraSerial.println("radio set bw 125");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  return true;
}

bool sendDirectionOnLoRa(char direction)
{
  Serial.print("packageID = ");
  Serial.println(packageID);

  if (direction == 'f')
  {
    loraSerial.print("radio tx ");
    loraSerial.println("66"); // f
  }
  else if (direction == 'b')
  {
    loraSerial.print("radio tx ");
    loraSerial.println("62"); // b
  }
  else if (direction == 'l')
  {
    loraSerial.print("radio tx ");
    loraSerial.println("6C"); // l
  }
  else if (direction == 'r')
  {
    loraSerial.print("radio tx ");
    loraSerial.println("72"); // r
  }
  else if (direction == 'x')
  {
    loraSerial.print("radio tx ");
    loraSerial.println("78"); // x
  }
  Serial.print("Sent direction: ");
  Serial.println(direction);

  // loraSerial.println(packageID);
  // str = loraSerial.readStringUntil('\n');
  // Serial.println(str);
  // str = loraSerial.readStringUntil('\n');
  // Serial.println(str);

  delay(50);
  packageID = packageID + 1;
  return true;
}

void lora_autobaud()
{
  String response = "";
  while (response == "")
  {
    loraSerial.write(0x00);
    loraSerial.write(0x55);
    loraSerial.println();
    loraSerial.println("sys get ver");
    response = loraSerial.readStringUntil('\n');
  }
}

/*
 * This function blocks until the word "ok\n" is received on the UART,
 * or until a timeout of 3*5 seconds.
 */
int wait_for_ok()
{
  str = loraSerial.readStringUntil('\n');
  if (str.indexOf("ok") == 0)
  {
    return 1;
  }
  else
    return 0;
}
