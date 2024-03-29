
#include <Arduino.h>
#include "settings.h"
//#include "printOnOled.h"
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"  // https://github.com/me-no-dev/ESPAsyncWebServer
#include <Wire.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <MQTT.h>
#include "connect.h"
#include <print_on_oled.h>
#include "TimeShowFormatted.h"
#include "createJson.h"
#include "ota.h"
#include "FS.h"
#include <ntp.h>
#include "LittleFS.h"
#include "htu21d.h"

MQTTClient client;
WiFiClient net;
AsyncWebServer server(80);

String date = "----";
time_t now;
int counter;
int oldcounter = 15000;
bool sensorPres = true;
const int interval = 30000;
const char mqttuser[] = MQTT_USERNAME;
const char mqttpass[] = MQTT_PASSWORD;

String topic = MQTT_PUB_TOPIC;
const char appname[50] = APPNAME;

char text_to_write_oled[55];
String tempTopic, htmldata, jsondata, myip;
char totTime[20];
char realDate[20];


void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.print(F("Welcome to "));
  Serial.println(APPNAME); 

  initOled();
  //char tmp[] = {APPNAME};
  //strcpy(text_to_write_oled, tmp);
  strcpy(text_to_write_oled, APPNAME);
  printoled(text_to_write_oled, 10, 20);
  delay(DISPLAY_TIME);
  clearOled();

  //const char ssid[] = MYSSID;
  strcpy (text_to_write_oled, "Connect to ");
  strcat (text_to_write_oled, MYSSID);
  Serial.println(text_to_write_oled);
  printoled(text_to_write_oled, 10, 20);
  delay(DISPLAY_TIME);
  clearOled();

  // Connect to WiFi
  myip = connectWifi();

  // Print IP address to Oled & Serial
  char __myip[sizeof(myip)];
  myip.toCharArray(__myip, 20);
  Serial.printf("Connected to Wifi with IP %s\n", __myip);

  printoled("IP: ", 10, 20); 
  printoled(__myip, 10, 40);
  delay(DISPLAY_TIME);
  clearOled();    

  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  // Set time  
  //setup_NTP();



  client.begin(MQTT_SERVER, net);
  client.setWill(MQTT_PUB_TOPIC, "Bye!");

  strcpy (text_to_write_oled, "Connect to MQTT server");
  Serial.println(text_to_write_oled);
  printoled(text_to_write_oled, 10, 20);
  delay(DISPLAY_TIME);
  clearOled();

  int connAttempts = 0;
  while (!client.connect(APPNAME, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.printf(". %i \n", connAttempts);
    connAttempts++;
    if (connAttempts>10) {
      Serial.print("MQTT connection error, check your settings.");
      strcpy (text_to_write_oled, "MQTT connection error");
      printoled(text_to_write_oled, 10, 20);
      delay(DISPLAY_TIME);
      clearOled();

      while(connAttempts > 10) {
        yield();
      }
    }
    delay(1000);
  }

  strcpy (text_to_write_oled, "Connected! ");
  strcat (text_to_write_oled, MYSSID);
  Serial.println(text_to_write_oled);
  printoled(text_to_write_oled, 10, 20);
  delay(DISPLAY_TIME);
  clearOled();

  Serial.print("Publishing to: ");
  Serial.println(MQTT_PUB_TOPIC);
  printoled(MQTT_PUB_TOPIC, 10, 20);
  delay(DISPLAY_TIME);
  clearOled();

  if (!client.connected()) {
    client.connect(APPNAME, MQTT_USERNAME, MQTT_PASSWORD);
  }
  String welmess = "Welcome to ";
  welmess += APPNAME;
  client.publish(MQTT_PUB_TOPIC, welmess);

  enableOTA();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", String(), false);
  });
  server.on("/json.html", HTTP_GET, [](AsyncWebServerRequest *request){
  });
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/style.css", "text/css");
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
  printoled(smess, 10, 20);
  delay(DISPLAY_TIME);
  clearOled();
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

      printoled(realDate, 10, 42);
      printoled(totTime, 10, 62);

      if (!client.connected()) {
        client.connect(APPNAME, MQTT_USERNAME, MQTT_PASSWORD);
      }
      tempTopic = topic + "/date";
      client.publish(tempTopic, realDate);
      tempTopic = topic + "/time";
      client.publish(tempTopic, totTime);

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

      //jsondata = createJson(appname,totTime,realDate,ctemp,chum,timenow,formTime,myip);
      Serial.print ("jsondata: ");
      Serial.println (jsondata);

      // Reset counter
      oldcounter=counter;
  }
}
