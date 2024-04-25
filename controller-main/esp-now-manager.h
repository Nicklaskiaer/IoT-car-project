#ifndef ESP_NOW_MANAGER_H
#define ESP_NOW_MANAGER_H

#include <esp_now.h>
#include <WiFi.h>


bool sendDirectionOnESPNow(char direction);
bool initESPNow();

#endif