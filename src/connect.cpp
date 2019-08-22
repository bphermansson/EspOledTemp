#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include "settings.h"

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
  Serial.println();
  Serial.print("Connected to Wifi with IP ");
  Serial.println(WiFi.localIP());
}
