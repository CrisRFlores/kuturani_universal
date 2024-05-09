#include <Arduino.h>
#include <Encoder.h>

Encoder myEnc(5, 17);

int EncoderSelection;
//int EncoderQuassiSelection_FirstLevel;
long oldPosition  = -999;
//int numberOfPresets_FirstLevel = 30;
//int FirstLevel_position = 0;
int counter;
int StepsPerStop = 4; // how many steps are counted each stop of the encoder


bool RotaryEncoder(byte sizemenu) {
  bool change = false;
  long newPosition = (myEnc.read()/4);
  if (newPosition != oldPosition) {
    if (newPosition > oldPosition){
      counter++;
    } else{
      counter--;
    }

    if(counter <=1){
      counter = 1;
    }
    if(counter >= sizemenu){
      counter = sizemenu;
    }
    Serial.println(counter);
    oldPosition = newPosition;
    change = true;
  }
  return change;
}



