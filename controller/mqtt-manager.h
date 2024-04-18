#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include "helpers.h"

#define MSG_BUFFER_SIZE (8)

void MQTTreconnect();
void MQTTcallback(char *topic, byte *payload, unsigned int length);
void MQTTpublishMessage(const char *topic, const char *payload, boolean retained);
bool sendDirectionOnMQTT(char direction);
bool initMQTT();

#endif