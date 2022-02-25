/**
 * @file htu21d.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <Arduino.h>
#include "Adafruit_HTU21DF.h"
Adafruit_HTU21DF htu = Adafruit_HTU21DF();

typedef struct 
{
    float temp;
    float humidity;
    uint8_t state;
} htu_values;

htu_values htuvalues = {0};

enum htustates 
{
    error,
    ok
};
//char ctemp[8];
//char chum[5];

void init_htu()
{
  // Init sensor
  if (!htu.begin()) 
  {  
    //strcpy (text_to_write_oled, "Sensor error");
    //Serial.println(text_to_write_oled);
    //printoled(text_to_write_oled, 10, 40);
    htuvalues.state = error;
  }
  else 
  {
    //strcpy (text_to_write_oled, "Sensor ok");
    //printoled(text_to_write_oled, 10, 40);
    //delay(DISPLAY_TIME);
    //clearOled();
    htuvalues.state = ok;
  }
}

htu_values read_htu()
{
    return htuvalues;
}
void update_htu()
{
  if(htuvalues.state != error)
  {
    htuvalues.temp = htu.readTemperature();
    htuvalues.humidity=htu.readHumidity();
  }
}

// Old code

/*
      // Temp & humidity
      if (sensorPres) {
        static float temp,hum;
        temp=htu.readTemperature();
        String stemp = String(temp);  // Dummy to easily measure variable length
        dtostrf(temp, stemp.length()-1, 1, ctemp);  // Float to string
        strcat(ctemp, "C");
        printoled(ctemp, 10, 15);

        hum=htu.readHumidity();
);
        sprintf(chum, "%02lf", hum); // Does this work instead of dtostrf?
        /*
        dtostrf(hum, 2, 0, chum);
        strcat(chum, "%");
        printoled(chum, 80, 15);
        */
       /*
        Serial.print("Temp: ");
        Serial.println(ctemp);
        Serial.print("Humidity: ");
        Serial.println(chum);
*/
        //tempTopic = topic + "/temp";
        //client.publish(tempTopic, ctemp);
        //tempTopic = topic + "/humidity";
        //client.publish(tempTopic, chum);
 /*
      }
      else {
        String err =  "No temp/humidity sensor attached";
        Serial.println(err);
        tempTopic = topic + "/err";
        client.publish(tempTopic, err);
      }
      */