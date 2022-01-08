/*
  printOnOled.cpp - Library for writing text to Oled
  Created by Patrik Hermansson, september 2021.
  Released into the public domain.
*/

#include "Arduino.h"
#include <U8g2lib.h>
#include "printOnOled.h"

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

#define BUFFERLEN 20

int totlen=0, pos=0, wordnr=1;

void my_fn(char *token, int x, int y)
{
  Serial.println(token);
  totlen+=strlen(token);
  Serial.println(totlen);
  if(totlen>15)
  {
    y+=18;
    wordnr=1;
    pos=0;
  }
  else
  {
    ;
  }
  Serial.println(pos);
  Serial.printf("Wordnr: %i\n", wordnr);
  wordnr++;
  u8g2.drawStr(pos, y, token);
  u8g2.sendBuffer();
  pos+=(u8g2.getStrWidth(token)+3);
}

void printoled(char text_to_write_oled[], int x, int y)
{
  Serial.begin(115200);
  Serial.println("Will soon print to oled: ");
  Serial.println(text_to_write_oled);
  

  u8g2.begin();
  delay(200);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_crox3h_tr);
  
  int txtLength=0, printlen=0, totprintlen=0;
  txtLength = strlen(text_to_write_oled);
  Serial.print("txtLength: ");
  Serial.println (txtLength);

// More general pattern:
const char *my_str_literal = "JAN,FEB,MAR";
char *token, *str, *tofree;

tofree = str = strdup(text_to_write_oled);  // We own str's memory now.
while ((token = strsep(&str, " "))) my_fn(token, x, y);
free(tofree);
totlen=0;
pos=0;   
    wordnr=1;

      Serial.printf("%d %d\n", x, y);
      //u8g2.drawStr(x, y, t);
      u8g2.sendBuffer();
   
  
  
}

