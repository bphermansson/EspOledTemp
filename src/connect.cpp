#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include "settings.h"

const char ssid[] = MYSSID;
const char pass[] = PASSWORD;

String connectWifi() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Connect to Wifi...");
  WiFi.persistent(false);  // Do not write Wifi settings to flash
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  int connAttempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(". %i \n", connAttempts);
    connAttempts++;
    if (connAttempts>15) {
      Serial.println("WiFi connection error, check your settings.");    
      Serial.println((String)"In settings ssid="+ssid+" and pass="+pass);      
      /*
      u8g2.clearBuffer();
      u8g2.drawStr( 1, 20, "WiFi connection ");
      u8g2.drawStr( 1, 35, "error");
      u8g2.sendBuffer();       
      */
      while(connAttempts > 15) {
        yield();
      }
    }
    delay(1000);
  }
  String conIP = WiFi.localIP().toString();
  return conIP;
}