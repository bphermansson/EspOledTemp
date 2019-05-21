#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "settings.h"

#include <MQTT.h>
MQTTClient client;

void connectWifi() {
  Serial.begin(115200);
  delay(400);
  Serial.println("Connect to Wifi...");
  WiFi.begin(MYSSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

}

void connect() {
  Serial.println("Connect to Mqtt broker...");
  while (!client.connect("appname", MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("Connected!");

  client.subscribe(mqtt_time_topic);
  // client.unsubscribe("/hello");
}
