#include "time.h"
#include <Arduino.h>
#include "settings.h"

bool setup_NTP() {
  configTime(GMTOFFSET, DAYLIGHTOFFSET, NTPSERVERNAME);

  Serial.println("\nWaiting for time");

  while (!time(nullptr))
  {
    Serial.print(".");
    delay(1000);
  }
  return true;
}
