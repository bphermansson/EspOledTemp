#include <Arduino.h>
#include <ArduinoJson.h>
#include <MQTT.h>
#include "settings.h"

void mqttPublish(MQTTClient client, float temp, float hum) {
  String mqtt_pub_topic = MQTT_PUB_TOPIC;
  const size_t capacity = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(3);
  DynamicJsonDocument values(capacity);
  values["temp"] = String(temp);
  values["humidity"] = hum;

  char output[128];
  serializeJson(values, output);
  Serial.print("Publish to Mqtt broker, subject: ");
  Serial.println(mqtt_pub_topic);
  Serial.println(output);
  client.publish(mqtt_pub_topic, output);

}
