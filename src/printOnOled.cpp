/*
  printOnOled.cpp - Library for writing text to Oled
  Created by Patrik Hermansson, september 2021.
  Released into the public domain.
*/

#include "Arduino.h"
#include <U8g2lib.h>
#include "printOnOled.h"

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void printoled(int x, int y)
{
  Serial.begin(115200);
  Serial.println("Will soon print to oled: ");
  //Serial.println(mess);

  u8g2.begin();
  delay(200);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_crox3h_tr);

  //int txtLength = strlen(textToWrite);
//  u8g2.drawStr( x, y, textToWrite);
  u8g2.sendBuffer();  // Without this the message won't display

int rowNr=0, c=0;
char txt[16][10];
int i;
/*
for ( i = 0; i<= txtLength; i++){
  txt[rowNr][c] = textToWrite[c];
  c++;
  
  if (c>=15) {
    Serial.println("Next row");
    //char testC[1];
    //strcpy (testC, " ");
    rowNr++;
  }

}
*/
  Serial.print("C: ");
  Serial.println(c);
  Serial.print("Nr of rows: ");
  Serial.println(rowNr);
  Serial.println("txtLength: ");
  //Serial.println(txtLength);
  Serial.println(x);


  c++;
  txt[rowNr][c] = '\n'; // Add a trailing new line to avoid printing garbage.
  Serial.print("row0 ");
  Serial.println(String(txt[0][0]));
/*
  Serial.println(15/14);
  Serial.println(15/15);
  Serial.println(15/16);
  Serial.println(txtLength/16);
*/
/*

  Serial.print("row2 ");
  Serial.println(row2);
  u8g2.drawStr( x, y, row1);
  u8g2.drawStr( x, y+20, row2);
  u8g2.sendBuffer(); 
*/

}