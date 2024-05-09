#include <Arduino.h>

const int analogInPin = 16; // Analog input pin that the potentiometer is attached to
int outputValue = 0;        
int oldPot;

int runningPot()
{
  outputValue = map(analogRead(analogInPin), 0, 1023, 0, 100);
  if //(outputValue < (oldPot * 0.9) || outputValue > (oldPot * 1.1)){
      (abs(outputValue - oldPot) > 4)
  {
    oldPot = outputValue;
    //Serial.println(outputValue);
  }
  return outputValue;
}
