#include <Arduino.h>
#include <Bounce2.h>

//////////////////// THIS FILE IS DEPRECATED. FUNCTIONS DECLARED IN MAIN, NO .H FILE NEEDED

// TRIGGER
if (Menu_Button.fell())
{
  // Serial.println("on");
}

int Menu_Button_triggerCount;
int Menu_Button_triggerOn_Off;

// TRIGGER ON-OFF
if (Menu_Button.changed())
{
   Menu_Button_triggerCount++;
   Menu_Button_triggerOn_Off = Menu_Button_triggerCount % 2;
  // Serial.println(Menu_Button_triggerOnOff);
}

int Menu_Button_toggleCount;
bool Menu_Button_toggleOn_Off;

// TOGGLE
if (Menu_Button.fell())
{
  Menu_Button_toggleCount++;
  if (Menu_Button_toggleCount % 2 == 0)
  {
    Menu_Button_toggleOn_Off = true;
  }
  else
  {
    Menu_Button_toggleOn_Off = false;
  }
  Serial.println(Menu_Button_toggleOn_Off);
}

//////////////////BOUNCE LIBRARY

/*
class MyButtons
{

  // class member variables
  int buttonPin;
  Bounce pushbutton;
  unsigned int count;
  bool OnOff;

  // constructor

public:
  MyButtons(int pin)
  {
    buttonPin = pin;
    pinMode(buttonPin, INPUT_PULLUP);
  }

  // class functions
  int pushbut()
  {
    if (pushbutton.update())
    {
      if (pushbutton.fallingEdge())
      {
        count = 1;
        //count = (count + 1);
       // Serial.println(count);
      }
    }
    return count;
  }

  int tooglebut()
  {
    if (pushbutton.update())
    {
      if (pushbutton.fallingEdge())
      {
        count = (count + 1);
        if (count % 2 == 0)
        {
          OnOff = true;
        }
        else
        {
          OnOff = false;
        }
       // Serial.println(OnOff);
      }
    }
    return OnOff;
  }
};
*/

////////////////////NO LIBRARY

/*
class MyButtons
{

  // class member variables
  int buttonPin;   // the number of the pushbutton pin
  int buttonState; // variable for reading the pushbutton status
  bool OnOff;
  int counter;
  int lastbuttonState;
  bool push;

  // constructor
public:
  MyButtons(int pin)
  {
    buttonPin = pin;
    pinMode(buttonPin, INPUT_PULLUP);
    buttonState = 0;
    OnOff = false;
    counter = 0;
    push = false;
  }
  // class function
  bool toggle()
  {
    buttonState = digitalRead(buttonPin);
    if (buttonState != lastbuttonState)
    {
      if (buttonState == HIGH)
      {
        counter++;
      }
      lastbuttonState = buttonState;

      if (counter % 2 == 0)
      {
        OnOff = true;
      }
      else
      {
        OnOff = false;
      }
    }
    // Serial.println(OnOff);
    return OnOff;
  }

  bool momentary()
  {
    buttonState = digitalRead(buttonPin);
    if (buttonState != lastbuttonState)
    {
      if (buttonState == HIGH)
      {
        push = true;
        Serial.println("picado");
      }
      lastbuttonState = buttonState;
    }

    return push;
  }
};

*/

/*
//IMPLEMENTATION

MyButtons mybutt(2);

void setup(){
}

void loop(){
  mybutt.update();
}

*/