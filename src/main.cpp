#include <Arduino.h>
#include "settings.h"
#include <ESP8266WiFi.h>
#include <MQTT.h> // MQTT by Joel Gaehwiler
#include <U8g2lib.h>
#include <Wire.h>
#include "Adafruit_HTU21DF.h"
#include <ArduinoJson.h>

MQTTClient client;
WiFiClient net;

#include "connect.h"
#include "ntp.h"
#include "mqttPublish.h"

//String date = "----";
char realDate[20];
float temp,hum;
time_t now;

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
  u8g2.drawStr( 10, 40, APPNAME);
  u8g2.sendBuffer();  // Without this the message won't display

  Serial.begin(115200);
  Serial.println();
  delay(100);

  Serial.print("Welcome to ");
  Serial.print(APPNAME);
  Serial.println("!");

  delay(2000);

  u8g2.clearBuffer();
  u8g2.drawStr( 10, 40, "Connecting...");
  u8g2.sendBuffer();  // Without this the message won't display

  connectWifi();

  delay(1000);

  while (!setup_NTP())
  {}

  u8g2.setFont( u8g2_font_crox1c_tf);     // Set Font
  // Init sensor
  if (!htu.begin()) {
  //if (htu.begin()) {
    char error[25];
    strcpy (error, "Sensor error");
    Serial.println(error);
    u8g2.clearBuffer();
    u8g2.drawStr( 10, 40, error);
    u8g2.sendBuffer();
    delay(5000);
    while (1){};
  }
  else {
    u8g2.clearBuffer();
    u8g2.drawStr( 10, 40, "Setup done");
    u8g2.sendBuffer();  // Without this the message won't display
    delay(1000);
  }
}

void loop() {
    client.loop();
    counter = millis();
    if (counter-oldcounter>interval){
      u8g2.clearBuffer();
      struct tm * timeinfo;
      time(&now);
      timeinfo = localtime(&now);
      char totTime[20];
      sprintf_P(totTime, (PGM_P)F("%02d:%02d"), timeinfo->tm_hour, timeinfo->tm_min);

      int realyear = timeinfo->tm_year + 1900;
      int realmon = timeinfo->tm_mon + 1;
      sprintf_P(realDate, (PGM_P)F("%04d-%02d-%02d"), realyear, realmon, timeinfo->tm_mday);

      Serial.println("----------------");

      Serial.print("Time: ");
      Serial.println(totTime);
      Serial.print("Date: ");
      Serial.println(realDate);

      u8g2.setFont(u8g2_font_crox3h_tr);
      u8g2.drawStr( 20, 20, realDate);

      u8g2.setFont(u8g2_font_logisoso16_tn);
      u8g2.drawStr( 40, 41, totTime);

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
      client.begin(MQTT_SERVER, net);
      while (!client.connect(APPNAME, mqttuser, mqttpass)) {
        Serial.print(".");
        delay(1000);
      }
    Serial.println("Connected!");
    }

    oldcounter=counter;
    Serial.print("Millis: ");
    Serial.println(counter);
  }
}
