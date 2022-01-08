#ifndef __PRINT_ON_OLED_H
#define __PRINT_ON_OLED_H

#include <Arduino.h>
// pragma once prevents the file to be included twice
#pragma once

// vars need to be extern
//extern String myName;
extern char text_to_write_oled[55];
//extern char *mess[];
//extern char *ptr_mess;
//extern String myName;

// prototypes don't need named attributes in the .h file

void printoled(char *text_to_write_oled, int x, int y);

#endif