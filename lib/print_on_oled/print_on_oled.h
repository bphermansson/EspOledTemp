#ifndef __PRINT_ON_OLED_H
#define __PRINT_ON_OLED_H

#include <Arduino.h>
// pragma once prevents the file to be included twice
#pragma once

// Vars need to be extern
extern char text_to_write_oled[55];

// prototypes don't need named attributes in the .h file
void initOled();
void clearOled();
void printoled(char *text_to_write_oled, int x, int y);

#endif