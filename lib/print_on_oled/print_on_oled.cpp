/*
  printOnOled.cpp - Library for writing text to Oled
  Created by Patrik Hermansson, september 2021.
  Released into the public domain.
*/

#include "Arduino.h"
#include <U8g2lib.h>
#include <print_on_oled.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

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
  u8g2.drawStr(0,10,"Hello World!!!");	// write something to the internal memory
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

/*
void printoled(char text_to_write_oled[], int x, int y)
{
  Serial.begin(115200);
  Serial.println("Will soon print to oled: ");
  Serial.println(text_to_write_oled);

  char *token, *str, *tofree;
  tofree = str = strdup(text_to_write_oled);  // We own str's memory now.
  while ((token = strsep(&str, " "))) my_fn(token, x, y);

  // Clean up and reset.
  //free(tofree);
  free(str);
  totlen=0;
  pos=0;   
  wordnr=1;
}
*/
void printoled(char text_to_write_oled[], int x, int y)
{
  Serial.begin(115200);
  Serial.println("Will soon print to oled: ");
  Serial.println(text_to_write_oled);

  if(strlen(text_to_write_oled) > 15) 
  {

      //char *token = text_to_write_oled;
      //char txt[400];
    //  token = strtok(text_to_write_oled, " ");
    //  strcpy(txt,token);
    //  strcpy(txt,token);

      char *pch;
      //char *wholeLine;
      
      int length = 100;
      //char *posts_array = (char*)malloc(length * sizeof(char));
      //char *ptr = (char*)malloc(length * sizeof(char));
      
      char buf[300];
      char arr_buf[5][300];
      uint8_t arr_row=0;
      
      printf ("Splitting string \"%s\" into tokens:\n",text_to_write_oled);
      pch = strtok (text_to_write_oled, " ,.-");
      strcpy(arr_buf[arr_row],pch);
      printf ("First part: %s\n",arr_buf[arr_row]);

      uint8_t buflen = 0;
      
      while (pch != NULL)
      {
        pch = strtok (NULL, " ,.-");
        if(pch != NULL) 
        {
          printf ("%d - %s\n", strlen(buf), pch);
          char *T = pch;
          //strcat(buf,T);
          strcat(arr_buf[arr_row],T);
          if (strlen(arr_buf[arr_row]) > 15)
          {
            arr_row++;
          }
        }
      }
      printf ("All the bits: %s\n",arr_buf[0]);
      printf ("All the bits: %s\n",arr_buf[1]);

      //printf ("buf: %s\n",buf);




      //printf( " %s\n", token ); 
      /*
      while( token != NULL ) {
        printf( " %s\n", token );  
        token = strtok(NULL, " ");
        strcpy(txt, token);
      }
      */
    /*
    while (txt = strtok(text_to_write_oled, " "))
    {
      printf( " %s\n", txt );
    }
    */

  }

/*
  char *token = strtok(text_to_write_oled, " ");
  while( token != NULL ) {
    printf( " %s\n", token ); //printing each token
    token = strtok(NULL, " ");
 //   strcpy(*txt, token);
  }
      //printf( "txt: %s\n", txt );
*/
}
