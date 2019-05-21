#include <Arduino.h>
#include "settings.h"
//#include "callback.h"
#include <ESP8266WiFi.h>
#include <MQTT.h>

#include <U8g2lib.h>
#include <Wire.h>
#include "Adafruit_HTU21DF.h"

#include <time.h>
#include <TimeLib.h>
#include <ArduinoJson.h>

#include "connect.h"

MQTTClient clientMain;

time_t t;
String totalTime;
String date = "----";
float temp,hum;
char msg[150];
const char* mqtt_value_topic = appname;

const int capacity = JSON_OBJECT_SIZE(3);
//StaticJsonDocument<capacity> doc;

// Declare devices
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
Adafruit_HTU21DF htu = Adafruit_HTU21DF();

void setup() {
  Serial.begin(115200);
  Serial.println();
  delay(1000);
  Serial.print("Welcome to ");
  Serial.print(appname);
  Serial.println("!");

  connectWifi();
  connect();

  u8g2.begin();
  if (!htu.begin()) {
    Serial.println("Couldn't find sensor!");
    // This will crash the poor Esp...
    while (1);
  }

}

void loop() {
  temp=htu.readTemperature();
  hum=htu.readHumidity();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_cupcakemetoyourleader_tr);

  char buff[12];
  date.toCharArray(buff, sizeof(buff));
  u8g2.drawStr( 20, 15, buff);

  if (minute()<10) {
        String zeromin = "0";     // Add trailing zero
        zeromin.concat(String(minute()));
        totalTime = String(hour()) + ":" + zeromin;
  }
  else {
    totalTime = String(hour()) + ":" + String(minute());
  }
  totalTime.toCharArray(buff, sizeof(buff));
  Serial.println(buff);

  u8g2.setFont(u8g2_font_logisoso16_tn);
  u8g2.drawStr( 40, 40, buff);

  u8g2.setFont(u8g2_font_cupcakemetoyourleader_tr);
  char ctemp[8];
  dtostrf(temp, 5, 1, ctemp);
  strcat(ctemp, "C");

  Serial.print("Temp: ");
  Serial.println(ctemp);

  u8g2.drawStr( 10, 60, ctemp);           // Display text 1

  char chum[5];
  dtostrf(hum, 5, 0, chum);
  strcat(chum, "%");

  Serial.print("Humidity: ");
  Serial.println(chum);

  u8g2.drawStr( 65, 60, chum);

  u8g2.sendBuffer();

  // Mqtt
  if (!clientMain.connected()) {
    connect();
  }

  //Serial.print("\t\tHum: "); Serial.println(hum);
  delay(5000);


}

/*
void conn() {
  WiFi.begin(MYSSID, PASSWORD);
  WiFi.hostname(appname);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  WiFi.mode(WIFI_STA);
  Serial.print("Connected to ");
  Serial.println(MYSSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup Mqtt connection
  while (!client.connect(appname, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.print(".");
    delay(1000);
  }
  client.subscribe(mqtt_time_topic);
  Serial.println("Mqtt setup done");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}
*/
