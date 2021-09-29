/*
  printOnOled.cpp - Library for writing text to Oled
  Created by Patrik Hermansson, september 2021.
  Released into the public domain.
*/

#include "Arduino.h"
#include <U8g2lib.h>
#include "printOnOled.h"

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

printOnOled::printOnOled(){
}

void printOnOled::printoled(int x, int y, char* textToWrite)
{
  Serial.begin(115200);
  //Serial.println(textToWrite);

  u8g2.begin();
  delay(200);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_crox3h_tr);

  int txtLength = strlen(textToWrite);

  const byte buflen = 10;
  char buf[ buflen ];
  
  int a = txtLength;    // Total length
  int b = 15;           // Length of each part
  int noOfParts = 0;    
  noOfParts = a / b +1; // How many parts?

  int xpos = 5;
  int ypos = 15;

  u8g2.drawStr( xpos, ypos, textToWrite);
  u8g2.sendBuffer();  // Without this the message won't display
/*
      Serial.println(textToWrite);

      Serial.println(txtLength);

      Serial.print("noOfParts: ");
      Serial.println(noOfParts);
*/
char row1[15]="";
char row2[15];

int noOfLines = (txtLength / 15) + 1;
      Serial.println("noOfLines: ");

      Serial.println(noOfLines);


int i=0, j=0;
for (i=0; i<15;i++) {
  row1[i] = textToWrite[i];
}
    u8g2.drawStr( x, y, textToWrite);

if (noOfLines > 1 && noOfLines < 3) {
  for (i=15; i<30;i++) {
    row2[j] = textToWrite[i];
    j++;
  } 
}

        //Serial.println(part1);
      Serial.print("row1 ");
      Serial.println(row1);
      Serial.print("row2 ");
      Serial.println(row2);

//}

  /*
  if (txtLength > 14) {
      //Serial.println(String(textToWrite).substring(0,14));
      //String tempTxt = String(textToWrite).substring(0,14);
      
      
    char* firstSegment = strtok(textToWrite, " ");
    char* eastSegment = strtok(NULL, "+");
    char* thirdSegment = strtok(NULL, "*");
    char* fourthSegment = strtok(NULL, "\0");


  Serial.println("f ");
  Serial.println(firstSegment);
  Serial.println("2 ");
  Serial.println(eastSegment);
  Serial.println(thirdSegment);
  Serial.println(fourthSegment); 
      

      Serial.println(String(textToWrite).substring(14));
      //tempTxt = String(textToWrite).substring(15);
      //u8g2.drawStr( x, y, tempTxt);

  }
  else {
    u8g2.drawStr( x, y, textToWrite);
  }
       

       Serial.println(txtLength);

*/

}