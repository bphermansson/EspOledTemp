#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include "settings.h"
#include "mqttrec.h"
//WiFiClient net;

const char ssid[] = MYSSID;
const char pass[] = PASSWORD;

void connectWifi() {
  Serial.begin(115200);
  Serial.println("Connect to Wifi...");
  WiFi.persistent(false);  // Do not write Wifi settings to flash
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected to Wifi with IP ");
  Serial.println(WiFi.localIP());
}
/*
int connectMqtt(MQTTClient client) {
  if (!client.connected()) {
    Serial.println("Connect to Mqtt broker...");
    client.begin(mqtt_server, net);
    while (!client.connect("appname", mqttuser, mqttpass)) {
      Serial.print(".");
      delay(1000);
    }
  Serial.println("Connected!");
  }
  return(0);
}
*/
