/*
  printOnOled.cpp - Library for writing text to Oled
  Created by Patrik Hermansson, september 2021.
  Released into the public domain.
*/

#include "Arduino.h"
#include <U8g2lib.h>
#include <print_on_oled.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
#define LENGTH 300

//#define BUFFERLEN 20
int totlen=0, pos=0, wordnr=1;

/*
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
*/
void initOled()
{
  Serial.printf("Init Oled.\n");
  u8g2.begin();
  u8g2.setFont(u8g2_font_crox3h_tr);
  u8g2_uint_t dh = u8g2.getDisplayHeight();
  u8g2_uint_t dw = u8g2.getDisplayWidth();
  Serial.printf("Oled size: %d x %d\n", dw, dh);
  u8g2.drawStr(0,10,"Hello World!");	// write something to the internal memory
  u8g2.sendBuffer();
  /*
  int cursX = u8g2.tx;
  int cursY = u8g2.ty;
  Serial.printf("X: %i Y: %i", cursX, cursY);
  */
  delay(3000);
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

    char oldbuf[10][300];
    char whole_text[20][20];
    int line_counter=0;
    char *pch; 
    char buf[LENGTH];
    memset(buf, 0, LENGTH);
    memset(oldbuf, 0, LENGTH);

    //strcpy (text_txt, "This is, a - test with a much longer text. It has more letters, more spaces, more of everything. But still is totally meaningless. ");
  
    pch = strtok (text_to_write_oled, " ,.-");    // Split text. 
    strcpy(buf,pch);                    // Store first part.
    strcpy(whole_text[line_counter], buf);  // Save first item
    strcat(whole_text[line_counter], " ");
    strcpy(oldbuf[line_counter], buf);      // Save old buffer

    while (pch != NULL)
    {
        pch = strtok (NULL, " ,-");
        if(pch != NULL) 
        {
            char *T = pch;
            strcat(buf,T);                         // Add next word  
            strcat(whole_text[line_counter], T);
            strcat(whole_text[line_counter], " ");

            if (strlen(whole_text[line_counter]) >= 15)   // Oops, got to long, go to next line.
            {
              //  printf ("%s - %lu\n", whole_text[line_counter], strlen(whole_text[line_counter]));
                //strcpy(whole_text[line_counter],oldbuf[line_counter]);  // Get back previous line
                line_counter++;
            }
            //strcat(buf,T);
            strcat(oldbuf[line_counter], T);
        }
        if(pch == NULL)
        {
            printf ("%s - %lu\n", whole_text[line_counter], strlen(whole_text[line_counter]));   // The last word
        }
    }

    memset(oldbuf, 0, LENGTH);

}
