
#include <Arduino.h>
#include "settings.h"
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include "FS.h"
#include <U8g2lib.h>
#include <Wire.h>
#include "Adafruit_HTU21DF.h"
#include <ArduinoJson.h>
#include <MQTT.h>
#include "connect.h"
#include "ntp.h"
 
String processor(const String& var);

MQTTClient client;
WiFiClient net;
AsyncWebServer server(80);

String date = "----";
float temp,hum;
time_t now;
int counter;
int oldcounter = 0;
bool sensorPres = true;
const int interval = 15000;
const char mqttuser[] = MQTT_USERNAME;
const char mqttpass[] = MQTT_PASSWORD;

String subTopic, htmldata;
char totTime[20];
char realDate[20];
char ctemp[8];
char chum[5];

void mqttPublish(String subTopic, String data);

// Declare devices
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
Adafruit_HTU21DF htu = Adafruit_HTU21DF();

void setup() {
  // Init display
  u8g2.begin();
  delay(200);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_crox3h_tr);
  u8g2.drawStr( 20, 20, "EspOledTemp");

  Serial.begin(115200);
  Serial.println();
  delay(100);

  Serial.print("Welcome to ");
  Serial.print(APPNAME);
  Serial.println("!");

  SPIFFS.begin();

  connectWifi();

  setup_NTP();

  // Init sensor
  if (!htu.begin()) {
    Serial.println("Couldn't find sensor!");
    sensorPres = false;
  }

  client.begin(MQTT_SERVER, net);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    //request->send(SPIFFS, "/index.html", String(), false, processor);
    request->send(SPIFFS, "/index.html", String(), false);

  });
  server.on("/json.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/json.html", String(), false, processor);
  });
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
  server.on("/readdata", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", htmldata);
  });
  server.begin();
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
      sprintf_P(totTime, (PGM_P)F("%02d:%02d"), timeinfo->tm_hour, timeinfo->tm_min);

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
        String stemp = String(temp);  // Dummy to easily measure variable length
        dtostrf(temp, stemp.length()-1, 1, ctemp);
        strcat(ctemp, "C");
        mqttPublish("/temp", ctemp);

        hum=htu.readHumidity();
        dtostrf(hum, 2, 0, chum);
        strcat(chum, "%");
        mqttPublish("/humidity", chum);

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

      // Create json-object from all data
      const size_t capacity = JSON_OBJECT_SIZE(8);
      DynamicJsonDocument doc(capacity);

      doc["appname"] = APPNAME;
      doc["time"] = totTime;
      doc["date"] = realDate;
      doc["temperature"] = ctemp;
      doc["humidity"] = chum;
      doc["uptime"] = millis();

      htmldata="";
      serializeJson(doc, Serial);
      serializeJson(doc, htmldata);

    oldcounter=counter;
    Serial.println(counter);
  }
}

void mqttPublish(String subTopic, String data) {
  String sTopic = MQTT_PUB_TOPIC+subTopic;
  const char *totTopic = sTopic.c_str();
  client.publish(totTopic, data);
}
String processor(const String& var){
  Serial.print("var: ");
  Serial.println(var);
  const size_t capacity = JSON_OBJECT_SIZE(7);
  DynamicJsonDocument doc(capacity);

  doc["appname"] = APPNAME;
  doc["time"] = totTime;
  doc["date"] = realDate;
  doc["temp"] = ctemp;
  doc["humidity"] = chum;

  serializeJson(doc, Serial);
  String output;
  serializeJson(doc, output);
  return output;
}
