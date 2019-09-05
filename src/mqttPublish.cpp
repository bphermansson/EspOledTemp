#include <Arduino.h>
#include "settings.h"
#include <MQTT.h>

extern MQTTClient client;
char totTopic[50];
const char *cTopic = MQTT_PUB_TOPIC;

//void mqttPublish(char *subTopic, char *data) {
void mqttPublish(String subTopic, String data) {
    //strcpy(totTopic,cTopic);
    //strcat(totTopic,subTopic);
    Serial.println("mqtt");
    Serial.println(subTopic);
    client.publish(totTopic, data);
    client.publish(MQTT_PUB_TOPIC+subTopic, data);
}
