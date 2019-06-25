#include <Arduino.h>
#include "settings.h"
#include <MQTT.h>

extern MQTTClient client;

String mqttPublish(char *subTopic, char *data) {
    char totTopic[30];
    const char *cTopic = MQTT_PUB_TOPIC;
    strcpy(totTopic,cTopic);
    strcat(totTopic,subTopic);
    client.publish(totTopic, data);
}
