int messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

/*void callback(char* topic, byte* payload, unsigned int length) {
  String stringPayload = "";
  const int offset = 0;   // Central European Time

  Serial.print("Message arrived with topic ");
  Serial.println(topic);
  Serial.println("Message: ");

  for (unsigned int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    stringPayload += (char)payload[i];
  }
  Serial.println();

  // Timestamp
  if (strcmp(topic,mqtt_time_topic)==0) {
    stringPayload = stringPayload.substring(0,10);  // Convert from timestamp in millis to ts i seconds

    t = stringPayload.toInt();
    setTime(t);
    adjustTime(offset * SECS_PER_HOUR);

    Serial.print("Got time via Mqtt: ");
    if (minute()<10) {
      //Serial.println("Less than 10");
      String zeromin = "0";     // Add trailing zero
      zeromin.concat(String(minute()));
      totalTime = String(hour()) + zeromin;
    }
    else {
      totalTime = String(hour()) + String(minute());
    }
    Serial.println(totalTime);

    String monthfixed;
    if (month()<10) {
       monthfixed = "0";     // Add trailing zero
       monthfixed.concat(String(month()));
    }
    else {
       monthfixed = (String(month()));
    }
    String dayfixed;
    if (day()<10) {
       dayfixed = "0";     // Add trailing zero
       dayfixed.concat(String(day()));
    }
    else {
       dayfixed = (String(day()));
    }
    date = String(year(t)) + "-" + monthfixed + "-"  + String(dayfixed);
    //strcpy (date, "-");
    // + month() + "-" + day(t);

    Serial.println(date);

  }
}
*/
