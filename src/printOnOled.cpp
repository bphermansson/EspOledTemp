/*
  printOnOled.cpp - Library for writing text to Oled
  Created by Patrik Hermansson, september 2021.
  Released into the public domain.
*/

#include "Arduino.h"
#include "printOnOled.h"

printOnOled::printOnOled(){
  //_textToWrite = textToWrite;
}

void printOnOled::printoled(String textToWrite)
{
    Serial.begin(115200);
    Serial.println(textToWrite);

}