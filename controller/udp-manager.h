#ifndef UDP_MANAGER_H
#define UDP_MANAGER_H

#include <WiFiUdp.h>
#include "helpers.h"


bool sendDirectionOnUDP(char direction);
bool initUDP();

#endif