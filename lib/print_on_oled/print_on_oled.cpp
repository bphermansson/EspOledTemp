/*
  printOnOled.cpp - Library for writing text to Oled
  Created by Patrik Hermansson, september 2021.
  Released into the public domain.
*/

#include "Arduino.h"
#include <U8g2lib.h>
#include <print_on_oled.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

int totlen=0, pos=0, wordnr=1;

void my_fn(char *token, int x, int y)
{
  totlen+=strlen(token);
  if(totlen>15)
  {
    y+=18;
    wordnr=1;
    pos=0;
  }
  else 
  {
    pos = x;
  }
  wordnr++;
  u8g2.drawStr(pos, y, token);
  u8g2.sendBuffer();
  pos+=(u8g2.getStrWidth(token)+3);
}

void initOled()
{
  Serial.printf("Init Oled.\n");
  u8g2.begin();
  u8g2.setFont(u8g2_font_crox3h_tr);
  u8g2_uint_t dh = u8g2.getDisplayHeight();
  u8g2_uint_t dw = u8g2.getDisplayWidth();
  Serial.printf("Oled size: %d x %d\n", dw, dh);

  int cursX = u8g2.tx;
  int cursY = u8g2.ty;
  Serial.printf("X: %i Y: %i", cursX, cursY);
}

void clearOled()
{
  u8g2.clearBuffer();
}

void printoled(char text_to_write_oled[], int x, int y)
{
  Serial.begin(115200);
  Serial.println("Will soon print to oled: ");
  Serial.println(text_to_write_oled);

  char *token, *str, *tofree;
  tofree = str = strdup(text_to_write_oled);  // We own str's memory now.
  while ((token = strsep(&str, " "))) my_fn(token, x, y);

  // Clean up and reset.
  free(str);
  totlen=0;
  pos=0;   
  wordnr=1;
}

