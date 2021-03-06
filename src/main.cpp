
#include <Arduino.h>
#include "settings.h"
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"  // https://github.com/me-no-dev/ESPAsyncWebServer
#include <U8g2lib.h>
#include <Wire.h>
#include "Adafruit_HTU21DF.h"
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <MQTT.h>
#include "connect.h"
#include "ntp.h"
#include "TimeShowFormatted.h"
#include "createJson.h"
#include "ota.h"
#include "FS.h"

MQTTClient client;
WiFiClient net;
AsyncWebServer server(80);

String date = "----";
float temp,hum;
time_t now;
int counter;
int oldcounter = 15000;
bool sensorPres = true;
const int interval = 15000;
const char mqttuser[] = MQTT_USERNAME;
const char mqttpass[] = MQTT_PASSWORD;
char appname[] = APPNAME;
String topic = MQTT_PUB_TOPIC;

String tempTopic, htmldata, jsondata, myip;
char totTime[20];
char realDate[20];
char ctemp[8];
char chum[5];

// Declare devices
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
Adafruit_HTU21DF htu = Adafruit_HTU21DF();

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {
  // Init display and print welcome message
  u8g2.begin();
  delay(200);
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

  u8g2.clearBuffer();
  u8g2.drawStr( 10, 40, "Connecting...");
  u8g2.sendBuffer();  // Without this the message won't display

  myip = connectWifi();

  delay(1000);
  SPIFFS.begin();                           // Start the SPI Flash Files System
  setup_NTP();

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
    u8g2.drawStr( 10, 40, "Sensor ok");
    u8g2.sendBuffer();  // Without this the message won't display
    delay(1000);
  }

  client.begin(MQTT_SERVER, net);
  client.setWill(MQTT_PUB_TOPIC, "Bye!");


  Serial.println("Connecting to MQTT broker");
  while (!client.connect(APPNAME, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connected!");
  Serial.print("Publishing to: ");
  Serial.println(MQTT_PUB_TOPIC);

  if (!client.connected()) {
    client.connect(APPNAME, MQTT_USERNAME, MQTT_PASSWORD);
  }
  String welmess = "Welcome to ";
  welmess += APPNAME;
  client.publish(MQTT_PUB_TOPIC, welmess);

  enableOTA();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false);
  });
  server.on("/json.html", HTTP_GET, [](AsyncWebServerRequest *request){
  });
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
  server.on("/readdata", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", jsondata);
  });
    server.onNotFound(notFound);

  server.begin();
  Serial.println("HTTP server started");

  char smess[15];
  strcpy(smess, "Setup done");
  Serial.println(smess);
  u8g2.clearBuffer();
  u8g2.drawStr( 10, 40, smess);
  u8g2.sendBuffer();  // Without this the message won't display
  delay(1000);
}

void loop() {
    ArduinoOTA.handle();
    client.loop();

    delay(10);  // <- fixes some issues with WiFi stability

    counter = millis();
    if (counter-oldcounter>interval){
      Serial.println("----------------");

      // Time & date
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
      u8g2.drawStr( 21, 13, realDate);  // x, y
      u8g2.setFont(u8g2_font_logisoso16_tn);
      u8g2.drawStr( 40, 41, totTime);

      if (!client.connected()) {
        client.connect(APPNAME, MQTT_USERNAME, MQTT_PASSWORD);
      }
      tempTopic = topic + "/date";
      client.publish(tempTopic, realDate);
      tempTopic = topic + "/time";
      client.publish(tempTopic, totTime);

      // Temp & humidity
      if (sensorPres) {
        temp=htu.readTemperature();
        String stemp = String(temp);  // Dummy to easily measure variable length
        dtostrf(temp, stemp.length()-1, 1, ctemp);
        strcat(ctemp, "C");

        hum=htu.readHumidity();
        dtostrf(hum, 2, 0, chum);
        strcat(chum, "%");

        Serial.print("Temp: ");
        Serial.println(ctemp);
        Serial.print("Humidity: ");
        Serial.println(chum);

        u8g2.setFont(u8g2_font_crox3h_tr);
        u8g2.drawStr( 20, 60, ctemp);
        u8g2.drawStr( 74, 60, chum);
        u8g2.sendBuffer();

        tempTopic = topic + "/temp";
        client.publish(tempTopic, ctemp);
        tempTopic = topic + "/humidity";
        client.publish(tempTopic, chum);
      }
      else {
        String err =  "No temp/humidity sensor attached";
        Serial.println(err);
        tempTopic = topic + "/err";
        client.publish(tempTopic, err);
      }

      long int timenow = millis();
      Serial.print("Uptime: ");
      Serial.println(timenow);
      tempTopic = topic + "/uptime";
      client.publish(tempTopic, String(timenow));

      String formTime = TimeShowFormatted(timenow); // Human readable uptime
      Serial.println(formTime);
      tempTopic = topic + "/uptimeHuman";
      client.publish(tempTopic, formTime);

      tempTopic = topic + "/ip";
      client.publish(tempTopic, myip);

      jsondata = createJson(appname,totTime,realDate,ctemp,chum,timenow,formTime,myip);
      Serial.print ("jsondata: ");
      Serial.println (jsondata);

      // Reset counter
      oldcounter=counter;
  }
}
