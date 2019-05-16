#include "settings.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "Adafruit_HTU21DF.h"
#include <PubSubClient.h>
#include <time.h>
#include <TimeLib.h>

#define appname "EspOledTemp"

time_t t;
String totalTime, date;
float temp,hum;
char msg[150];
const char* mqtt_server = "192.168.1.190";
const char* mqtt_time_topic = "time";
const char* mqtt_value_topic = appname;

// Declare devices
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
Adafruit_HTU21DF htu = Adafruit_HTU21DF();
WiFiClient espClient;
PubSubClient client(espClient);

#include "reconnect.h"
#include "callback.h"

void setup() {
//    Wire.begin();
  Serial.begin(115200);
  Serial.println();
  delay(1000);
  Serial.print("Welcome to ");
  Serial.print(appname);
  Serial.println("!");

  // Wifi, code from WifiManagerAutoConnect example
  WiFi.begin(MYSSID, PASSWORD);
  WiFi.hostname(appname);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(MYSSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup Mqtt connection
  client.setServer(mqtt_server, 1883);
  if (!client.connected()) {
      mqttreconnect();
  }
  client.setCallback(callback);
  Serial.println("Mqtt done");

  u8g2.begin();
  if (!htu.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }

}

void loop() {
  temp=htu.readTemperature();
  hum=htu.readHumidity();
  u8g2.clearBuffer();
  //u8g2.setFont(u8g2_font_fub14_tf);
  u8g2.setFont(u8g2_font_cupcakemetoyourleader_tr);

  char buff[12];
  date.toCharArray(buff, sizeof(buff));
  u8g2.drawStr( 20, 10, buff);


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
  if (!client.connected()) {
    mqttreconnect();
  }
  client.loop();

  //Serial.print("\t\tHum: "); Serial.println(hum);
  delay(5000);


}
