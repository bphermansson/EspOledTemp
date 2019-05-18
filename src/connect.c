void connect() {
  #include "settings.h"

  // Wifi, code from WifiManagerAutoConnect example
  espClient.begin(MYSSID, PASSWORD);
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
  while (!client.connect(appname, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.print(".");
    delay(1000);
  }
  client.subscribe(mqtt_time_topic);
  Serial.println("Mqtt setup done");


}
