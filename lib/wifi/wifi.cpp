#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "wifi.h"
#include "../../settings.h"

void wifi_connect(int *ipAddrPtr)
{   
  WiFi.begin(SSID, PASSWORD);
  #ifdef DEBUG
    Serial.print("Connecting to Wifi");
  #endif

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    #ifdef DEBUG
      Serial.print(".");
    #endif
  }
    #ifdef DEBUG
      Serial.println();
      Serial.print("Connected, IP address: ");
      Serial.println(WiFi.localIP());
    #endif
}