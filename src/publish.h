void publish() {
// Publish values from local sensor

    char msg[100];
    jsonmess["intemp"] = chtutemp;
    jsonmess["inhumidity"] = chtuhum;
    jsonmess["eCO2"]= eCO2;
    jsonmess["TVOC"]= TVOC;
    jsonmess.printTo((char*)msg, root.measureLength() + 1);
    client.publish(mqtt_value_topic, msg, true);  // Send with retain=true
}
