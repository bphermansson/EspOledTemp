#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include "settings.h"

const char ssid[] = MYSSID;
const char pass[] = PASSWORD;

String connectWifi() {
  Serial.begin(115200);
  Serial.println("Connect to Wifi...");
  WiFi.persistent(false);  // Do not write Wifi settings to flash
  WiFi.mode(WIFI_STA);
  WiFi.begin("BrandstorpWifi", "Brandstorp");

  Serial.println("Connect to Wifi!!!...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  IPAddress ipaddr;
  //ipaddr = WiFi.localIP();
  //ip = ipaddr.toString();

  Serial.println();
  Serial.print("Connected to Wifi with IP ");
  String conIP = WiFi.localIP().toString();
  Serial.println(conIP);
  return conIP;
  //return ip;
}
