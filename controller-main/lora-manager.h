#ifndef LORA_MANAGER_H
#define LORA_MANAGER_H

#include <HardwareSerial.h>
#include "helpers.h"

bool initLoRa();
bool sendDirectionOnLoRa(char direction);
void lora_autobaud();
int wait_for_ok();

#endif