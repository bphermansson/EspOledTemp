#include <Arduino.h>

String TimeShowFormatted(int ms) {
  long days = 0;
  long hours = 0;
  long mins = 0;
  long secs = 0;
  String secs_o = ":";
  String mins_o = ":";
  String hours_o = ":";
  secs = ms / 1000; // set the seconds remaining
  mins = secs / 60; //convert seconds to minutes
  hours = mins / 60; //convert minutes to hours
  days = hours / 24; //convert hours to days
  secs = secs - (mins * 60); //subtract the coverted seconds to minutes in order to display 59 secs max
  mins = mins - (hours * 60); //subtract the coverted minutes to hours in order to display 59 minutes max
  hours = hours - (days * 24); //subtract the coverted hours to days in order to display 23 hours max
  if (secs < 10) {
    secs_o = ":0";
  }
  if (mins < 10) {
    mins_o = ":0";
  }
  if (hours < 10) {
    hours_o = ":0";
  }
  return days + hours_o + hours + mins_o + mins + secs_o + secs;

}
