#include <Arduino.h>
#include "settings.h"
#include <ESP8266WiFi.h>
//#include <MQTT.h> // MQTT by Joel Gaehwiler
#include <U8g2lib.h>
#include <Wire.h>
#include "Adafruit_HTU21DF.h"
//#include <time.h>                       // time() ctime()
//#include <sys/time.h>                   // struct timeval
//#include <coredecls.h>
#include <ArduinoJson.h>
#include <MQTT.h>
MQTTClient client;
WiFiClient net;

#include "connect.h"
#include "ntp.h"
#include "mqttPublish.h"

String date = "----";
float temp,hum;
time_t now;
//timeval tv;
//timespec tp;

int counter;
int oldcounter = 0;
const int interval = 15000;
const char mqttuser[] = MQTT_USERNAME;
const char mqttpass[] = MQTT_PASSWORD;

// Declare devices
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
Adafruit_HTU21DF htu = Adafruit_HTU21DF();

void setup() {
  // Init display
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_crox3h_tr);
  u8g2.drawStr( 40, 20, "EspOledTemp");

  Serial.begin(115200);
  Serial.println();
  delay(100);

  Serial.print("Welcome to ");
  Serial.print(appname);
  Serial.println("!");

  connectWifi();

  setup_NTP();

  // Init sensor
  if (!htu.begin()) {
    Serial.println("Couldn't find sensor!");
    // This will crash the poor Esp...
    while (1);
  }
}
/*
#define PTM(w) \
  Serial.print(":" #w "="); \
*/
//    printTm("localtime", localtime(&now));
/*
void printTm(const char* what, const tm* tm) {
  Serial.print(what);
  PTM(isdst); PTM(yday); PTM(wday);
  PTM(year);  PTM(mon);  PTM(mday);
  PTM(hour);  PTM(min);  PTM(sec);
}
*/
void loop() {
    client.loop();
    counter = millis();
    if (counter-oldcounter>interval){
      u8g2.clearBuffer();
      //time_t now;
      struct tm * timeinfo;
      time(&now);
      timeinfo = localtime(&now);
      // Struct described at https://github.com/esp8266/Arduino/blob/master/tools/sdk/libc/xtensa-lx106-elf/include/time.h
  /*    int hournow = timeinfo->tm_hour;
      int minnow = timeinfo->tm_min;
      int yr = timeinfo->tm_year;
      int mnt = timeinfo->tm_mon;
      int dy = timeinfo->tm_mday;
      String totalTime = String(hournow) + ":" + String(minnow);

      char buffTime[12];
      totalTime.toCharArray(buffTime, sizeof(buffTime));
      Serial.println(buffTime);
  */
      char totTime[20];
      sprintf_P(totTime, (PGM_P)F("%02d:%02d"), timeinfo->tm_hour, timeinfo->tm_min);

      char realDate[20];
      int realyear = timeinfo->tm_year + 1900;
      int realmon = timeinfo->tm_mon + 1;
      sprintf_P(realDate, (PGM_P)F("%04d-%02d-%02d"), realyear, realmon, timeinfo->tm_mday);

      Serial.print("totTime: ");
      Serial.println(totTime);
      Serial.print("realDate: ");
      Serial.println(realDate);

      u8g2.setFont(u8g2_font_crox3h_tr);
      u8g2.drawStr( 20, 20, realDate);

      u8g2.setFont(u8g2_font_logisoso16_tn);
      u8g2.drawStr( 40, 41, totTime);

  /*
      if (timeStatus() != timeNotSet) {
        char totDate[20];
        sprintf_P(totDate, (PGM_P)F("%02d-%02d-%02d"), year(), month(), day());
        Serial.println(totDate);

        String zerohour = "0";
        String zeromin = "0";

        if (hour()<10) {
          zerohour.concat(String(hour()));
        }
        else {
          zeromin = String(hour());
        }

        if (minute()<10) {
          // Add trailing zero
          zeromin.concat(String(minute()));
        }
        else {
          zeromin = String(minute());
        }

        totalTime = zerohour + ":" + zeromin;


    }
  */
    temp=htu.readTemperature();
    char ctemp[8];
    dtostrf(temp, 5, 1, ctemp);
    strcat(ctemp, "C");

    Serial.print("Temp: ");
    Serial.println(ctemp);

    u8g2.setFont(u8g2_font_crox3h_tr);
    u8g2.drawStr( 10, 60, ctemp);

    hum=htu.readHumidity();
    char chum[5];
    dtostrf(hum, 5, 0, chum);
    strcat(chum, "%");
    Serial.print("Humidity: ");
    Serial.println(chum);
    u8g2.drawStr( 65, 60, chum);
    u8g2.sendBuffer();

    // Mqtt
    if (!client.connected()) {
      Serial.println("Connect to Mqtt broker...");
      client.begin(mqtt_server, net);
      while (!client.connect(appname, mqttuser, mqttpass)) {
        Serial.print(".");
        delay(1000);
      }
    Serial.println("Connected!");
    }

    oldcounter=counter;
    //counter = 0;
    Serial.println(counter);
  }
}
