#include <ESP8266WiFi.h>
#include "settings.h"

void setup_NTP()
{
    configTime(GMTOFFSET, DAYLIGHTOFFSET, NTPSERVERNAME);
    Serial.println("Setup time done");
}
