void mqttreconnect() {
  // Loop until we're reconnected
//    Serial.begin(115200);

  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(appname, MQTT_USERNAME, MQTT_PASSWORD)) {
      snprintf (msg, 150, "Connected to Mqtt broker as %s", appname);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish(mqtt_value_topic, msg);
      client.subscribe(mqtt_time_topic);
      Serial.print("Subscribed to: ");
      Serial.println(mqtt_time_topic);
    } else {
      Serial.print("Mqtt connection failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
