#include "time.h"
#include <Arduino.h>
#include "settings.h"

char buffer[80];

void setup_NTP()
{
  time_t rawtime;
  struct tm * timeinfo;

  configTime(GMTOFFSET, DAYLIGHTOFFSET, NTPSERVERNAME);

  Serial.println("\nWaiting for time");
  while (!time(nullptr))
  {
    Serial.print(".");
    delay(1000);
  }
  delay(1000);

  Serial.println("Time set, moving on");

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  strftime (buffer,80," %d %B %Y %H:%M:%S ",timeinfo);
  Serial.println(buffer);
}
