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

/*
#include <ESP8266WiFi.h>
#include <time.h>                       // time() ctime()
#include <sys/time.h>                   // struct timeval
#include <coredecls.h>                  // settimeofday_cb()
#include "settings.h"

#define TZ          1       // (utc+) TZ in hours
#define DST_MN      60      // use 60mn for summer time in some countries
#define TZ_MN       ((TZ)*60)
#define TZ_SEC      ((TZ)*3600)
#define DST_SEC     ((DST_MN)*60)
#define TIME_ZONE   localTZ
//#define NTP_SERVER  ntpServerName

timeval cbtime;			// time set in callback
bool cbtime_set = false;
timeval tv;
timespec tp;
//time_t now;
int cnt;

void timeSet_cb(void)
{
    gettimeofday(&cbtime, NULL);
    cbtime_set = true;
    cnt++;
}

void setup_NTP()
{
    cbtime_set = false;
    settimeofday_cb(timeSet_cb);
    timezone tz = {0, 0};
    settimeofday(&tv, &tz);
    setenv("TZ", TIME_ZONE, 1);
    tzset();
    configTime(0, 0, NTP_SERVER);
}
*/
