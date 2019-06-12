#include <Arduino.h>
#include "settings.h"
#include <ESP8266WiFi.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "Adafruit_HTU21DF.h"
#include <ArduinoJson.h>
#include <MQTT.h>
#include "connect.h"
#include "ntp.h"

MQTTClient client;
WiFiClient net;

String date = "----";
float temp,hum;
time_t now;
int counter;
int oldcounter = 0;
bool sensorPres = true;
const int interval = 15000;
const char mqttuser[] = MQTT_USERNAME;
const char mqttpass[] = MQTT_PASSWORD;
String subTopic;

void mqttPublish(String subTopic, String data);

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
  Serial.print(APPNAME);
  Serial.println("!");

  connectWifi();

  setup_NTP();

  // Init sensor
  if (!htu.begin()) {
    Serial.println("Couldn't find sensor!");
    sensorPres = false;
  }

  client.begin(MQTT_SERVER, net);

}

void loop() {
    client.loop();
    delay(10);  // <- fixes some issues with WiFi stability
    counter = millis();
    if (counter-oldcounter>interval){
      // Mqtt
      if (!client.connected()) {
        Serial.println("Connect to Mqtt broker...");
        client.begin(MQTT_SERVER, net);
        while (!client.connect(APPNAME, MQTT_USERNAME, MQTT_PASSWORD)) {
          Serial.print(".");
          delay(1000);
        }
        Serial.println("Connected!");
      }

      struct tm * timeinfo;
      time(&now);
      timeinfo = localtime(&now);
      char totTime[20];
      sprintf_P(totTime, (PGM_P)F("%02d:%02d"), timeinfo->tm_hour, timeinfo->tm_min);

      char realDate[20];
      int realyear = timeinfo->tm_year + 1900;
      int realmon = timeinfo->tm_mon + 1;
      sprintf_P(realDate, (PGM_P)F("%04d-%02d-%02d"), realyear, realmon, timeinfo->tm_mday);

      Serial.print("Time: ");
      Serial.println(totTime);
      Serial.print("Date: ");
      Serial.println(realDate);

      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_crox3h_tr);
      u8g2.drawStr( 20, 20, realDate);
      u8g2.setFont(u8g2_font_logisoso16_tn);
      u8g2.drawStr( 40, 41, totTime);

      mqttPublish("/date", realDate);
      mqttPublish("/time", totTime);

      if (sensorPres) {
        temp=htu.readTemperature();
        char ctemp[8];
        dtostrf(temp, 5, 1, ctemp);
        strcat(ctemp, "C");

        hum=htu.readHumidity();
        char chum[5];
        dtostrf(hum, 5, 0, chum);
        strcat(chum, "%");

        Serial.print("Temp: ");
        Serial.println(ctemp);
        u8g2.setFont(u8g2_font_crox3h_tr);
        u8g2.drawStr( 10, 60, ctemp);

        Serial.print("Humidity: ");
        Serial.println(chum);
        u8g2.drawStr( 65, 60, chum);
        u8g2.sendBuffer();
      }
      else {
        Serial.println ("No temp/humidity sensor attached");
      }

    oldcounter=counter;
    Serial.println(counter);
  }
}

void mqttPublish(String subTopic, String data) {
  String sTopic = MQTT_PUB_TOPIC+subTopic;
  const char *totTopic = sTopic.c_str();
  client.publish(totTopic, data);
}
