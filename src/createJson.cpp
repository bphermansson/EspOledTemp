//#include <MQTT.h>
#include <ArduinoJson.h>

String createJson(String appname, char *totTime, char *realDate, char *ctemp, char *chum, long uptime) {
  const size_t capacity = JSON_OBJECT_SIZE(9);
  DynamicJsonDocument doc(capacity);

  doc["appname"] = appname;
  doc["time"] = totTime;
  doc["date"] = realDate;
  doc["temp"] = ctemp;
  doc["humidity"] = chum;
  doc["uptime"] = uptime;

  //serializeJson(doc, Serial);
  String output;
  serializeJson(doc, output);
  return output;
}
