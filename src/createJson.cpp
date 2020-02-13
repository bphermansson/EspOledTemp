#include <ArduinoJson.h>

String createJson(String appname, char *totTime, char *realDate, char *ctemp, char *chum, long uptime, String formTime, String myip) {
  const size_t capacity = JSON_OBJECT_SIZE(13);
  DynamicJsonDocument doc(capacity);

  doc["appname"] = appname;
  doc["time"] = totTime;
  doc["date"] = realDate;
  doc["temp"] = ctemp;
  doc["humidity"] = chum;
  doc["uptime"] = uptime;
  doc["formTime"] = formTime;
  doc["myip"] = myip;

  //serializeJson(doc, Serial);
  String output;
  serializeJson(doc, output);
  return output;
}
