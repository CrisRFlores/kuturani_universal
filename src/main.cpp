//@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@
//========================  KUTURANI CON TEENSY 4.0  ===++++++===========//
//========================  LMS not integrated!!!!  ===++++++===========//
//========================  Cristohper Ramos Flores - UNAM   ===++++++===========//
//@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@



//@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@
//================================  SETUP PROGRAM  ==============================//
//@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@

//==================================================//
//=================  LIBRARIES  ====================//
//==================================================//

#include <Arduino.h>
#include <EEPROM.h>
#include <Encoder.h>
#include <Bounce2.h>
#include "displayandmenus.h"
#include "rotary_encoder.h"
#include "pot.h"
#include "accel.h"
#include "menu.h"
#include "eepromstuff.h"
// #include "buttons.h"

//==================================================//
//==============  AUDIO  VARIABLES  ================//
//==================================================//

/*
* Audio Shield Connections to Teensy 4
 * GND
 * AGND (next to 3.3V pin)
 * MCLK pin 23
 * BCLK pin 21
 * LRCLK pin 20
 * SCL pin 19
 * SDA pin 18
 * DOUT (Audio shield to Teensy) pin 8
 *
 * Note conflicts on Audio Shield:
 * DIN (Teensy to Audio Shield) - Intereferes with the HUB75 signals - you need to cut the jumper on the Audio Shield to disconnect pin 7 or otherwise not connect that pin to the SmartLED Shield
 * SDCS, MOSI, MISO, SCK, MEMCS - don't use the shield's SD Card slot or onboard memory chip.  If you need to use an SD card use the Teesny 4.1 or external SD module
 * 
 * I2S Microphone connections to Teensy 4
 * GND - GND
 * VDD - 3V3
 * SCK - 21 (BCLK)
 * SD  - 8 (DOUT)
 * WS  - 20 (LRCLK)
 * L/R set to ground (left) or 3V3 (right) to choose which channel the I2S microphone drives
 * 
 * I2S Microphone connections to Teensy 3
 * GND - GND
 * VDD - 3V3
 * SCK - 9 (BCLK)
 * SD  - 13 (DOUT)
 * WS  - 23 (LRCLK)
 * L/R set to ground (left) or 3V3 (right) to choose which channel the I2S microphone drives
 * 
 * If you want to use Line In instead of the electret mic input with the audio shield, adjust the `myInput` setting below
 *   I haven't tested Line In, if you have, please let me know so I can update this example!
 *
 * This SmartMatrix example uses just the Background layer
 */

//////////////////////////////////////////////////////////////////////////////////////////////////

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S i2sIn;                // xy=115,142
AudioMixer4 InvertedSignal;         // xy=283,154
AudioEffectBitcrusher bitcrusher1;  // xy=461,209
AudioEffectChorus chorus1;          // xy=597,209
AudioEffectFlange flange1;          // xy=722,209
AudioEffectGranular granular1;      // xy=858,209
AudioAmplifier dry_instrument_gain; // xy=987,156
AudioEffectFreeverb freeverb1;      // xy=997,209
AudioMixer4 Reverb_dry_wet_mixer;   // xy=1218,176
AudioOutputI2S i2sOut;              // xy=1422,184
AudioOutputUSB usb1;                // xy=1424,225
AudioConnection patchCord1(i2sIn, 0, InvertedSignal, 0);
AudioConnection patchCord2(InvertedSignal, dry_instrument_gain);
AudioConnection patchCord3(InvertedSignal, bitcrusher1);
AudioConnection patchCord4(bitcrusher1, chorus1);
AudioConnection patchCord5(chorus1, flange1);
AudioConnection patchCord6(flange1, granular1);
AudioConnection patchCord7(granular1, freeverb1);
AudioConnection patchCord8(dry_instrument_gain, 0, Reverb_dry_wet_mixer, 0);
AudioConnection patchCord9(freeverb1, 0, Reverb_dry_wet_mixer, 1);
AudioConnection patchCord10(Reverb_dry_wet_mixer, 0, i2sOut, 0);
AudioConnection patchCord11(Reverb_dry_wet_mixer, 0, i2sOut, 1);
AudioConnection patchCord12(Reverb_dry_wet_mixer, 0, usb1, 0);
AudioConnection patchCord13(Reverb_dry_wet_mixer, 0, usb1, 1);
AudioControlSGTL5000 sgtl5000_1; // xy=123,79
// GUItool: end automatically generated code

//////////////////////////////////////////////////////////////////////////////////////////////////

//==================================================//
//============ AUDIO MANUAL VARIABLES ==============//
//==================================================//

// bitcrusher variables
int CrushBits = 16; // 16 defaults to passthrough.
int SampleRate = 1; // 44100 defaults to passthrough. See srateConverted = 44100/SampleRate;
int srateConverted;

int eeprom_CrushBits = 0;
int eeprom_SampleRate = 1;

// chorus variables
#define CHORUS_DELAY_LENGTH (2 * AUDIO_BLOCK_SAMPLES) // 16 * 128 --16
short chorusdelayline[CHORUS_DELAY_LENGTH];
int n_chorus = 1;

int eeprom_chorusdelayline = 2;
int eeprom_n_chorus = 3;

// flange variables
#define FLANGE_DELAY_LENGTH (2 * AUDIO_BLOCK_SAMPLES) // 6 --10
short flangedelayline[FLANGE_DELAY_LENGTH];
int offset = 1;
int offsetConverted = offset * FLANGE_DELAY_LENGTH / 4; // 2*FLANGE_DELAY_LENGTH / 4;
int depthConverted = 1;
int depth = 2 * FLANGE_DELAY_LENGTH / 4;
int delayRateInt = 1; // 3;
float delayRateFloat = .1;

// int delayRateFor_eeprom = 1;

int eeprom_offset = 4;
int eeprom_depth = 5;
int eeprom_delayRate = 6;

// granular
#define GRANULAR_MEMORY_SIZE 4000 // enough for 290 ms at 44.1 kHz --8000
int16_t granularMemory[GRANULAR_MEMORY_SIZE];

float ratio = 1.0;
int grainLenght = 1;
int ratioInt = 1;

int eeprom_ratio = 7;
int eeprom_grainLenght = 8;

// freeverb

float roomsizeFloat = 0.1;
float dampingFloat = 0.1;
int roomsizeInt = 1;
int dampingInt = 1;

int eeprom_roomsize = 9;
int eeprom_damping = 10;

// waveform/combine
// int current_waveform=0;

//==================================================//
//==============  GLOBAL VARIABLES  ================//
//==================================================//

// button variables

// MyButtons menuButton(0);  ---> not used
// MyButtons encoderButton(1);
// MyButtons forwardButton(2);
// MyButtons backwardButton(3);
// MyButtons effectOffButton(4);
#define EDIT_BUTTON_PIN 0
#define ENCODER_BUTTON_PIN 1
#define BUTTON1_PIN 2
#define BUTTON2_PIN 3
#define BUTTON3_PIN 4

// INSTANTIATE A Button OBJECT FROM THE Bounce2 NAMESPACE
Bounce2::Button Edit_Button = Bounce2::Button();
Bounce2::Button Encoder_Button = Bounce2::Button();
Bounce2::Button Button1 = Bounce2::Button();
Bounce2::Button Button2 = Bounce2::Button();
Bounce2::Button Button3 = Bounce2::Button();

int Edit_Button_triggerCount;
int Edit_Button_triggerOn_Off;
int Edit_Button_toggleCount;
bool Edit_Button_toggleOn_Off;

int Encoder_Button_triggerCount;
int Encoder_Button_triggerOn_Off;
int Encoder_Button_toggleCount;
bool Encoder_Button_toggleOn_Off;

int Button1_triggerCount;
int Button1_triggerOn_Off;
int Button1_toggleCount;
bool Button1_toggleOn_Off;

int Button2_triggerCount;
int Button2_triggerOn_Off;
int Button2_toggleCount;
bool Button2_toggleOn_Off;

int Button3_triggerCount;
int Button3_triggerOn_Off;
int Button3_toggleCount;
bool Button3_toggleOn_Off;

// encoder --> moved to rotary_encoder.h

// display
#define CMD_ONE_COLUMN_SCROLL_H_LEFT 0x2D
int x = 0; // set position for text scrolling
int minX;  // set end of text scroll position

// menu system
bool editionInProgress = false;
bool enterMenu = false;
unsigned long previousMillis = 0; // timing to change menu
const long interval = 300;        // how long before changing to customMenu_activated
unsigned long currentMillis;

int thisisatest;

//==================================================//
//===================  EEPROM  =====================//
//==================================================//

// ----> moved to eepromstuff.h

//==================================================//
//============  FORWARD DECLARATIONS  ==============//
//==================================================//

int floatToIntX_00(float val);
float intToFloatX_00(int val);
int floatToIntX_0(float val);
float intToFloatX_0(int val);
bool RotaryEncoderEditor_F(float &currentParamF, float minValueF, float maxValueF);
bool RotaryEncoderEditor_I(int &currentParam, int minValue, int maxValue);

//@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@
//================================  SETUP PROGRAM  ==============================//
//@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@

void setup()
{
  //==================================================//
  //================ SPLASH SCREEN ===================//
  //==================================================//

  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(100);
  display.clearDisplay();
  delay(100);
  testdrawbitmap(); // Draw a small bitmap image in "display.h"
  delay(2000);
  display.clearDisplay();
  display.display();

  //==================================================//
  //=================  GYROSCOPE  ====================//
  //==================================================//

  MPU6050setup();

  //==================================================//
  //==================  ENCODER  =====================//
  //==================================================//

  // encoder.begin(17, 5, 24, 30); ---> rotary_encoder.h

  //==================================================//
  //==================  BUTTONS  =====================//
  //==================================================//

  Edit_Button.attach(EDIT_BUTTON_PIN, INPUT_PULLUP);
  Encoder_Button.attach(ENCODER_BUTTON_PIN, INPUT_PULLUP);
  Button1.attach(BUTTON1_PIN, INPUT_PULLUP);
  Button2.attach(BUTTON2_PIN, INPUT_PULLUP);
  Button3.attach(BUTTON3_PIN, INPUT_PULLUP);
  Edit_Button.interval(15);
  Encoder_Button.interval(15);
  Button1.interval(15);
  Button2.interval(15);
  Button3.interval(15);

  //==================================================//
  //===================  AUDIO  ======================//
  //==================================================//

  AudioMemory(12);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.8);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC); // or AUDIO_INPUT_MIC using INMP441
  sgtl5000_1.micGain(30);
  InvertedSignal.gain(0, 1); //(0, -0.5)
  //========== bitcrusher
  srateConverted = 44100 / SampleRate;
  bitcrusher1.bits(CrushBits);
  bitcrusher1.sampleRate(srateConverted);

  //========== chorus
  n_chorus = EEPROM.read(eeprom_n_chorus);
  n_chorus = 1;
  chorus1.begin(chorusdelayline, CHORUS_DELAY_LENGTH, n_chorus);

  //========== flange
  offset = EEPROM.read(eeprom_offset);
  depth = EEPROM.read(eeprom_depth);
  delayRateInt = EEPROM.read(eeprom_delayRate);
  // Serial.print("delayRateforeeprom "); Serial.print(delayRateFor_eeprom);Serial.print("delayRate "); Serial.println(delayRate);
  offsetConverted = offset * FLANGE_DELAY_LENGTH / 4;
  depthConverted = depth * FLANGE_DELAY_LENGTH / 4;
  // delayRate = delayRateConverted;
  delayRateFloat = delayRateInt * .01;
  flange1.begin(flangedelayline, FLANGE_DELAY_LENGTH, offsetConverted, depthConverted, delayRateFloat);

  //========== granular
  ratioInt = EEPROM.read(eeprom_ratio);
  ratio = ratioInt * .01;
  grainLenght = EEPROM.read(eeprom_grainLenght);

  granular1.begin(granularMemory, GRANULAR_MEMORY_SIZE);

  //========== freeverb
  roomsizeInt = EEPROM.read(eeprom_roomsize);
  roomsizeFloat = roomsizeInt * .01;
  dampingInt = EEPROM.read(eeprom_damping);
  dampingFloat = dampingInt * .01;

  // freeverb1.roomsize(0.0);
  // freeverb1.damping(0.0);

  // waveform / combine ====
  // waveform1.frequency(10);
  // waveform1.amplitude(1.0);
  // current_waveform = WAVEFORM_SQUARE;
  // waveform1.begin(current_waveform);
  // combine1.setCombineMode(AudioEffectDigitalCombine::MODULO);

  //=========== default connections
  patchCord3.disconnect();
  patchCord4.disconnect();
  patchCord5.disconnect();
  patchCord6.disconnect();
  patchCord7.disconnect();
  patchCord9.disconnect();

  //=========== display
  display.setTextWrap(false);

  // delay(1000);
  Serial.begin(9600);
}

//@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@
//================================   MAIN PROGRAM  ==============================//
//@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@###############@@@@@@@@@@@@@@@@@
float thiisafloattest = 0.09;
int thiisaninttest = 2;
void loop()
{

  runningPot();
  MPU6050loop();

  Edit_Button.update(); //  .fell() -> trigger 1 // .rose() -> 0 // .read() -> change
  Encoder_Button.update();
  Button1.update();
  Button2.update();
  Button3.update();
  
/*
  if (Edit_Button.fell())
  {
    Serial.println(" edit on");
  }
  */

  //======= dry instrument gain
  dry_instrument_gain.gain(0.0); // range 0.0 - 1.0
  //======= bitcrusher
  // bitcrusher1.bits(1);
  //======= chorus
  // chorus1.voices(3);
  //======= flange
  flange1.voices(offset, depth, delayRateFloat);
  //======= granuar
  granular1.setSpeed(1.);
  if (Button1.fell()) // Button1.fell() //digitalRead(2 == HIGH)
  {
    Serial.println(" granular on");

    granular1.beginFreeze(random(50, 200));
  }
  // granular1.setSpeed(map(float(outputValue), 0.0, 100.0, 0.0, 1.0));
  // granular1.beginPitchShift(map(float(outputValue), 0.0, 100.0, 0.0, 5.0));
  //======= reverb
  // freeverb1.roomsize(0.5);          // 0 - 1.0  ======== map(outputValue, 0, 100, 0, 1.0 )
  // freeverb1.damping(0.1);           // 0 - 1.0
  // Reverb_dry_wet_mixer.gain(1, map(float(outputValue), 0.0, 100.0, 0.0, 1.0));
  Reverb_dry_wet_mixer.gain(2, 1.); //(2, map(float(outputValue), 0.0, 100.0, 1.0, 0.0))
  // Serial.println(map(float(outputValue), 0.0, 100.0, 0.0, 1.0 ));

  //==================================================//
  //===================  MENUS  ======================//
  //==================================================//

  // menu display
  ///////////////////////// MAIN MENU //////////////////////////
  ///////////////////////// MAIN MENU //////////////////////////

  if (SelectedMenu == 0) // Main menu options
  {
    display.clearDisplay();
    display.fillRect(0, 0, 128, 16, WHITE);
    display.setTextSize(2);
    display.setTextColor(0);
    display.setCursor(23, 1);
    display.print("PRESETS");
    display.setTextColor(WHITE);
    currentMillis = millis();

    if (RotaryEncoder(sizeMainMenu))
    {
      if (counter == 1)
      {
        display.setTextSize(2);
        display.setCursor(0, 17);
        display.print(MainMenu[counter - 1]);
        display.setTextSize(1);
        display.setCursor(0, 33);
        display.print(MainMenu[counter]);
        display.setCursor(0, 41);
        display.print(MainMenu[counter + 1]);
        display.setCursor(0, 49);
        display.print(MainMenu[counter + 2]);
        display.setCursor(0, 57);
        display.print(MainMenu[counter + 3]);
      }

      if (counter == 2)
      {
        display.setTextSize(1);
        display.setCursor(0, 17);
        display.print(MainMenu[counter - 2]);
        display.setTextSize(2);
        display.setCursor(0, 25);
        display.print(MainMenu[counter - 1]);
        display.setTextSize(1);
        display.setCursor(0, 41);
        display.print(MainMenu[counter]);
        display.setCursor(0, 49);
        display.print(MainMenu[counter + 1]);
        display.setCursor(0, 57);
        display.print(MainMenu[counter + 2]);
      }

      if (counter == 3)
      {
        display.setTextSize(1);
        display.setCursor(0, 17);
        display.print(MainMenu[counter - 3]);
        display.setCursor(0, 25);
        display.print(MainMenu[counter - 2]);
        display.setTextSize(2);
        display.setCursor(0, 33);
        display.print(MainMenu[counter - 1]);
        display.setTextSize(1);
        display.setCursor(0, 49);
        display.print(MainMenu[counter]);
        display.setCursor(0, 57);
        display.print(MainMenu[counter + 1]);
      }

      if (counter == 4) //(counter > 3 && counter < 6)
      {
        display.setTextSize(1);
        display.setCursor(0, 17);
        display.print(MainMenu[counter - 3]);
        display.setCursor(0, 25);
        display.print(MainMenu[counter - 2]);
        display.setTextSize(2);
        display.setCursor(0, 33);
        display.print(MainMenu[counter - 1]);
        display.setTextSize(1);
        display.setCursor(0, 49);
        display.print(MainMenu[counter]);
        display.setCursor(0, 57);
        display.print(MainMenu[counter + 1]);
      }

      if (counter == 5)
      {
        display.setTextSize(1);
        display.setCursor(0, 17);
        display.print(MainMenu[counter - 4]);
        display.setCursor(0, 25);
        display.print(MainMenu[counter - 3]);
        display.setCursor(0, 33);
        display.print(MainMenu[counter - 2]);
        display.setTextSize(2);
        display.setCursor(0, 41);
        display.print(MainMenu[counter - 1]);
        display.setTextSize(1);
        display.setCursor(0, 57);
        display.print(MainMenu[counter]);
      }
      if (counter == 6)
      {
        display.setTextSize(1);
        display.setCursor(0, 17);
        display.print(MainMenu[counter - 5]);
        display.setCursor(0, 25);
        display.print(MainMenu[counter - 4]);
        display.setCursor(0, 33);
        display.print(MainMenu[counter - 3]);
        display.setCursor(0, 41);
        display.print(MainMenu[counter - 2]);
        display.setTextSize(2);
        display.setCursor(0, 49);
        display.print(MainMenu[counter - 1]);
      }
      display.display();
    }
  }

  ///////////////////////// MAIN MENU //////////////////////////
  ///////////////////// AUDIO CONNECTION ///////////////////////

  if (SelectedMenu == 0 && counter == 1 && Encoder_Button.fell()) // bitcrusher
  {
    // audio
    patchCord3.disconnect();
    patchCord4.disconnect();
    patchCord5.disconnect();
    patchCord6.disconnect();
    patchCord7.disconnect();
    patchCord9.disconnect();
    patchCord3.connect(InvertedSignal, bitcrusher1);
    patchCord4.connect(bitcrusher1, 0, Reverb_dry_wet_mixer, 1);
    Serial.println("bitcrusher");
    // display
    chosenPresetMainMenu(1);
  }

  if (SelectedMenu == 0 && counter == 2 && Encoder_Button.fell()) // chorus
  {
    // audio
    patchCord3.disconnect();
    patchCord4.disconnect();
    patchCord5.disconnect();
    patchCord6.disconnect();
    patchCord7.disconnect();
    patchCord9.disconnect();
    patchCord3.connect(InvertedSignal, chorus1);
    patchCord4.connect(chorus1, 0, Reverb_dry_wet_mixer, 1);
    Serial.println("chorus");
    // display
    chosenPresetMainMenu(2);
  }

  if (SelectedMenu == 0 && counter == 3 && Encoder_Button.fell()) // flange
  {
    // audio
    patchCord3.disconnect();
    patchCord4.disconnect();
    patchCord5.disconnect();
    patchCord6.disconnect();
    patchCord7.disconnect();
    patchCord9.disconnect();
    patchCord3.connect(InvertedSignal, flange1);
    patchCord4.connect(flange1, 0, Reverb_dry_wet_mixer, 1);
    Serial.println("flange");
    // display
    chosenPresetMainMenu(3);
  }

  if (SelectedMenu == 0 && counter == 4 && Encoder_Button.fell()) // granular
  {
    // audio
    patchCord3.disconnect();
    patchCord4.disconnect();
    patchCord5.disconnect();
    patchCord6.disconnect();
    patchCord7.disconnect();
    patchCord9.disconnect();
    patchCord3.connect(InvertedSignal, granular1);
    patchCord4.connect(granular1, 0, Reverb_dry_wet_mixer, 1);
    Serial.println("granular");
    // display
    chosenPresetMainMenu(4);
  }

  if (SelectedMenu == 0 && counter == 5 && Encoder_Button.fell()) // reverb
  {
    // audio
    patchCord3.disconnect();
    patchCord4.disconnect();
    patchCord5.disconnect();
    patchCord6.disconnect();
    patchCord7.disconnect();
    patchCord9.disconnect();
    patchCord3.connect(InvertedSignal, freeverb1);
    patchCord4.connect(freeverb1, 0, Reverb_dry_wet_mixer, 1);
    Serial.println("reverb");
    // display
    chosenPresetMainMenu(5);
  }

  if (SelectedMenu == 0 && counter == 6 && Encoder_Button.rose()) // custom menu
  {
    SelectedMenu = 1;
    myEnc.write(1);
    counter = 1;
  }

  // custom menu display
  /////////////////////// CUSTOM MENU //////////////////////////
  /////////////////////// CUSTOM MENU //////////////////////////

  if (SelectedMenu == 1) // Main menu options
  {
    display.clearDisplay();
    display.fillRect(0, 0, 128, 16, WHITE);
    display.setTextSize(2);
    display.setTextColor(0);
    display.setCursor(23, 1);
    display.print("CUSTOM");
    display.setTextColor(WHITE);

    if (RotaryEncoder(sizecustomMenu))
    {
      if (counter == 1)
      {
        display.setTextSize(2);
        display.setCursor(0, 17);
        display.print(customMenu[counter - 1]);
        display.setTextSize(1);
        display.setCursor(0, 33);
        display.print(customMenu[counter]);
        display.setCursor(0, 41);
        display.print(customMenu[counter + 1]);
        display.setCursor(0, 49);
        display.print(customMenu[counter + 2]);
        display.setCursor(0, 57);
        display.print(customMenu[counter + 3]);
      }

      if (counter == 2)
      {
        display.setTextSize(1);
        display.setCursor(0, 17);
        display.print(customMenu[counter - 2]);
        display.setTextSize(2);
        display.setCursor(0, 25);
        display.print(customMenu[counter - 1]);
        display.setTextSize(1);
        display.setCursor(0, 41);
        display.print(customMenu[counter]);
        display.setCursor(0, 49);
        display.print(customMenu[counter + 1]);
        display.setCursor(0, 57);
        display.print(customMenu[counter + 2]);
      }

      if (counter == 3)
      {
        display.setTextSize(1);
        display.setCursor(0, 17);
        display.print(customMenu[counter - 3]);
        display.setCursor(0, 25);
        display.print(customMenu[counter - 2]);
        display.setTextSize(2);
        display.setCursor(0, 33);
        display.print(customMenu[counter - 1]);
        display.setTextSize(1);
        display.setCursor(0, 49);
        display.print(customMenu[counter]);
        display.setCursor(0, 57);
        display.print(customMenu[counter + 1]);
      }

      if (counter > 3 && counter < 25) //(counter > 3 && counter < 6)
      {
        display.setTextSize(1);
        display.setCursor(0, 17);
        display.print(customMenu[counter - 3]);
        display.setCursor(0, 25);
        display.print(customMenu[counter - 2]);
        display.setTextSize(2);
        display.setCursor(0, 33);
        display.print(customMenu[counter - 1]);
        display.setTextSize(1);
        display.setCursor(0, 49);
        display.print(customMenu[counter]);
        display.setCursor(0, 57);
        display.print(customMenu[counter + 1]);
      }

      if (counter == 25)
      {
        display.setTextSize(1);
        display.setCursor(0, 17);
        display.print(customMenu[counter - 4]);
        display.setCursor(0, 25);
        display.print(customMenu[counter - 3]);
        display.setCursor(0, 33);
        display.print(customMenu[counter - 2]);
        display.setTextSize(2);
        display.setCursor(0, 41);
        display.print(customMenu[counter - 1]);
        display.setTextSize(1);
        display.setCursor(0, 57);
        display.print(customMenu[counter]);
      }
      if (counter == 26)
      {
        display.setTextSize(1);
        display.setCursor(0, 17);
        display.print(customMenu[counter - 5]);
        display.setCursor(0, 25);
        display.print(customMenu[counter - 4]);
        display.setCursor(0, 33);
        display.print(customMenu[counter - 3]);
        display.setCursor(0, 41);
        display.print(customMenu[counter - 2]);
        display.setTextSize(2);
        display.setCursor(0, 49);
        display.print(customMenu[counter - 1]);
      }
      display.display();
    }
  }

  /////////////////////// CUSTOM MENU //////////////////////////
  ///////////////////// AUDIO CONNECTION ///////////////////////

  if (SelectedMenu == 1 && counter == 1 && Encoder_Button.fell()) // // custom 1
  {
    chosenPresetCustomMenu(1);
  }

  if (SelectedMenu == 1 && counter == 2 && Encoder_Button.fell()) // custom 2
  {
    chosenPresetCustomMenu(2);
  }
  if (SelectedMenu == 1 && counter == 3 && Encoder_Button.fell()) // custom 3
  {
    chosenPresetCustomMenu(3);
  }
  if (SelectedMenu == 1 && counter == 4 && Encoder_Button.fell()) // custom 4
  {
    chosenPresetCustomMenu(4);
  }
  if (SelectedMenu == 1 && counter == 5 && Encoder_Button.fell()) // custom 5
  {
    chosenPresetCustomMenu(5);
  }
  if (SelectedMenu == 1 && counter == 6 && Encoder_Button.fell()) // custom 6
  {
    chosenPresetCustomMenu(6);
  }
  if (SelectedMenu == 1 && counter == 7 && Encoder_Button.fell()) // custom 7
  {
    chosenPresetCustomMenu(7);
  }
  if (SelectedMenu == 1 && counter == 8 && Encoder_Button.fell()) // custom 8
  {
    chosenPresetCustomMenu(8);
  }
  if (SelectedMenu == 1 && counter == 9 && Encoder_Button.fell()) // custom 9
  {
    chosenPresetCustomMenu(9);
  }
  if (SelectedMenu == 1 && counter == 10 && Encoder_Button.fell()) // custom 10
  {
    chosenPresetCustomMenu(10);
  }
  if (SelectedMenu == 1 && counter == 11 && Encoder_Button.fell()) // // custom 11
  {
    chosenPresetCustomMenu(11);
  }
  if (SelectedMenu == 1 && counter == 12 && Encoder_Button.fell()) // custom 12
  {
    chosenPresetCustomMenu(12);
  }
  if (SelectedMenu == 1 && counter == 13 && Encoder_Button.fell()) // custom 13
  {
    chosenPresetCustomMenu(13);
  }
  if (SelectedMenu == 1 && counter == 14 && Encoder_Button.fell()) // custom 14
  {
    chosenPresetCustomMenu(14);
  }
  if (SelectedMenu == 1 && counter == 15 && Encoder_Button.fell()) // custom 15
  {
    chosenPresetCustomMenu(15);
  }
  if (SelectedMenu == 1 && counter == 16 && Encoder_Button.fell()) // custom 16
  {
    chosenPresetCustomMenu(16);
  }
  if (SelectedMenu == 1 && counter == 17 && Encoder_Button.fell()) // custom 17
  {
    chosenPresetCustomMenu(17);
  }
  if (SelectedMenu == 1 && counter == 18 && Encoder_Button.fell()) // custom 18
  {
    chosenPresetCustomMenu(18);
  }
  if (SelectedMenu == 1 && counter == 19 && Encoder_Button.fell()) // custom 19
  {
    chosenPresetCustomMenu(19);
  }
  if (SelectedMenu == 1 && counter == 20 && Encoder_Button.fell()) // custom 20
  {
    chosenPresetCustomMenu(20);
  }
  if (SelectedMenu == 1 && counter == 21 && Encoder_Button.fell()) // custom 21
  {
    chosenPresetCustomMenu(21);
  }
  if (SelectedMenu == 1 && counter == 22 && Encoder_Button.fell()) // custom 22
  {
    chosenPresetCustomMenu(22);
  }
  if (SelectedMenu == 1 && counter == 23 && Encoder_Button.fell()) // custom 23
  {
    chosenPresetCustomMenu(23);
  }
  if (SelectedMenu == 1 && counter == 24 && Encoder_Button.fell()) // custom 24
  {
    chosenPresetCustomMenu(24);
  }
  if (SelectedMenu == 1 && counter == 25 && Encoder_Button.fell()) // custom 25
  {
    chosenPresetCustomMenu(25);
  }

  // GO BACK TO MAIN MENU
  if (SelectedMenu == 1 && counter == 26 && Encoder_Button.fell()) // bitcrusher
  {
    myEnc.write(1);
    counter = 1;
    SelectedMenu = 0;
  }

  //////////////////////// EDIT MENUS //////////////////////////
  //////////////////////// EDIT MENUS //////////////////////////

  // 1 enter edit menu
  if (SelectedMenu == 0 && counter == 1 && Edit_Button.fell()) // bitcrusher
  {
    myEnc.write(1);
    counter = 0;
    editMenu(2, "Bitcrusher"); // (SelectedMenu, "display name")
  }
  // 2 display edit menu
  if (SelectedMenu == 2)
  {
    display.clearDisplay();
    display.fillRect(0, 0, 128, 16, WHITE);
    display.setTextSize(2);
    display.setTextColor(BLACK);
    display.setCursor(5, 1);
    display.print("BITCRUSHER");
    display.setTextColor(WHITE);

    if (RotaryEncoder(sizebitcrusherMenu))
    {
      if (counter == 1)
      {
        display.setTextSize(2);
        display.setCursor(0, 17);
        display.print(bitcrusherMenu[counter - 1]); // parameter
        display.setCursor(xValuePos - 5, 17);
        display.print(CrushBits); // parameter's value
        display.setTextSize(1);
        display.setCursor(0, 38);
        display.print(bitcrusherMenu[counter]); // parameter
        display.setCursor(xValuePos - 5, 38);
        display.print(srateConverted); // parameter's value
        display.setTextSize(1);
        display.setCursor(0, 54);
        display.print(bitcrusherMenu[counter + 1]); // parameter
      }
      if (counter == 2)
      {
        display.setTextSize(1);
        display.setCursor(0, 17);
        display.print(bitcrusherMenu[counter - 2]); // parameter
        display.setCursor(xValuePos - 5, 17);
        display.print(CrushBits); // parameter's value
        display.setTextSize(2);
        display.setCursor(0, 30);
        display.print("S.R."); // parameter
        display.setCursor(xValuePos - 30, 30);
        display.print(srateConverted); // parameter's value
        display.setTextSize(1);
        display.setCursor(0, 54);
        display.print(bitcrusherMenu[counter]); // parameter
      }
      if (counter == 3)
      {
        display.setTextSize(1);
        display.setCursor(0, 17);
        display.print(bitcrusherMenu[counter - 3]); // parameter
        display.setCursor(xValuePos - 5, 17);
        display.print(CrushBits); // parameter's value
        display.setCursor(0, 30);
        display.print(bitcrusherMenu[counter - 2]); // parameter
        display.setCursor(xValuePos - 5, 30);
        display.print(srateConverted); // parameter's value
        display.setTextSize(2);
        display.setCursor(0, 46);
        display.print(bitcrusherMenu[counter - 1]); // parameter
      }
      display.display();
    }
  }
  // back to previous menu
  if (SelectedMenu == 2 && counter == 3 && Encoder_Button.fell()) //
  {
    myEnc.write(1);
    counter = 1;
    SelectedMenu = 0;
  }
  // 3 edit parameters
  if (SelectedMenu == 2 && counter == 1 && Encoder_Button.rose()) // change menu so that parameter edition happens
  {
    SelectedMenu = 1021; // new menu for edition <=====
    myEnc.write(1);
    counter = 1;
  }
  if (SelectedMenu == 1021) // display and save new parameter's value
  {
    editing();
    if (RotaryEncoderEditor_I(CrushBits, 0, 15))
    {
      bitcrusher1.bits(CrushBits); // parameter change <=============
      display.setTextSize(2);
      display.setCursor(0, 33);
      display.print(bitcrusherMenu[counter - 1]); // parameter
      display.setCursor(xValuePos, 33);
      display.print(CrushBits); // parameter's value
      display.display();
    }
    if (Edit_Button.fell()) // save parameter! <=====
    {
      // add eemprom
      EEPROM.write(eeprom_CrushBits, CrushBits);
      SelectedMenu = 2; // back to previous menu <====
      myEnc.write(1);
      counter = 1;
    }
  }

  if (SelectedMenu == 2 && counter == 2 && Encoder_Button.rose()) // change menu so that parameter edition happens
  {
    SelectedMenu = 1022; // new menu for edition <=====
    myEnc.write(1);
    counter = 1;
  }
  if (SelectedMenu == 1022) // display and save new parameter's value
  {
    editing();
    if (RotaryEncoderEditor_I(SampleRate, 0, 20))
    {
      srateConverted = 44100 / SampleRate;
      bitcrusher1.sampleRate(srateConverted); // parameter change <=============
      display.setTextSize(2);
      display.setCursor(0, 33);
      display.print(bitcrusherMenu[counter - 1]); // parameter
      display.setCursor(xValuePos - 30, 33);
      display.print(srateConverted); // parameter's value
      display.display();
    }
    if (Edit_Button.fell()) // save parameter! <=====
    {
      // add eemprom
      EEPROM.write(eeprom_SampleRate, srateConverted);
      SelectedMenu = 2; // back to previous menu <====
      myEnc.write(1);
      counter = 1;
    }
  }

  ////////////////

  // 1 enter edit menu
  if (SelectedMenu == 0 && counter == 2 && Edit_Button.fell()) // chorus
  {
    myEnc.write(1);
    counter = 0;
    editMenu(3, "Chorus"); // (SelectedMenu, "display name")
  }
  // 2 display edit menu
  if (SelectedMenu == 3)
  {
    display.clearDisplay();
    display.fillRect(0, 0, 128, 16, WHITE);
    display.setTextSize(2);
    display.setTextColor(BLACK);
    display.setCursor(5, 1);
    display.print("CHORUS");
    display.setTextColor(WHITE);

    if (RotaryEncoder(sizechorusMenu))
    {
      if (counter == 1)
      {
        display.setTextSize(2);
        display.setCursor(0, 26);
        display.print(chorusMenu[counter - 1]); // parameter
        display.setCursor(xValuePos, 26);
        display.print(n_chorus); // parameter's value
        display.setTextSize(1);
        display.setCursor(0, 44);
        display.print(chorusMenu[counter]); // parameter
        // display.setCursor(xValuePos, 44);
        // display.print("30"); // parameter's value
      }
      if (counter == 2)
      {
        display.setTextSize(1);
        display.setCursor(0, 26);
        display.print(chorusMenu[counter - 2]); // parameter
        display.setCursor(xValuePos, 26);
        display.print(n_chorus); // parameter's value
        display.setTextSize(2);
        display.setCursor(0, 38);
        display.print(chorusMenu[counter - 1]); // parameter
        // display.setCursor(xValuePos, 38);
        // display.print("30"); // parameter's value
      }
      display.display();
    }
  }
  // back to previous menu
  if (SelectedMenu == 3 && counter == 2 && Encoder_Button.fell()) //
  {
    myEnc.write(1);
    counter = 1;
    SelectedMenu = 0;
  }
  // 3 edit parameters
  if (SelectedMenu == 3 && counter == 1 && Encoder_Button.rose()) // change menu so that parameter edition happens
  {
    SelectedMenu = 103; // new menu for edition <=====
    myEnc.write(1);
    counter = 1;
  }
  if (SelectedMenu == 103) // display and save new parameter's value
  {
    editing();
    if (RotaryEncoderEditor_I(n_chorus, 0, 16))
    {
      chorus1.voices(n_chorus); // parameter change <=============
      display.setTextSize(2);
      display.setCursor(0, 33);
      display.print(chorusMenu[counter - 1]); // parameter
      display.setCursor(xValuePos, 33);
      display.print(n_chorus); // parameter's value
      display.display();
    }
    if (Edit_Button.fell()) // save parameter! <=====
    {
      // add eemprom
      EEPROM.write(eeprom_n_chorus, n_chorus);
      SelectedMenu = 3; // back to previous menu <====
      myEnc.write(1);
      counter = 1;
    }
  }

  ////////////////

  // 1 enter edit menu
  if (SelectedMenu == 0 && counter == 3 && Edit_Button.fell()) // flanger
  {
    myEnc.write(1);
    counter = 0;
    editMenu(4, "Flanger"); // (SelectedMenu, "display name")
  }
  // 2 display edit menu
  if (SelectedMenu == 4)
  {
    display.clearDisplay();
    display.fillRect(0, 0, 128, 16, WHITE);
    display.setTextSize(2);
    display.setTextColor(BLACK);
    display.setCursor(5, 1);
    display.print("FLANGER");
    display.setTextColor(WHITE);

    if (RotaryEncoder(sizeflangerMenu))
    {
      if (counter == 1)
      {
        display.setTextSize(2);
        display.setCursor(0, 18);
        display.print(flangerMenu[counter - 1]); // parameter
        display.setCursor(xValuePos - 18, 18);
        display.print(offsetConverted); // parameter's value
        display.setTextSize(1);
        display.setCursor(0, 35);
        display.print(flangerMenu[counter]); // parameter
        display.setCursor(xValuePos - 18, 35);
        display.print(depthConverted); // parameter's value
        display.setCursor(0, 45);
        display.print(flangerMenu[counter + 1]); // parameter
        display.setCursor(xValuePos - 18, 45);
        display.print(delayRateFloat); // parameter's value
        display.setCursor(0, 55);
        display.print(flangerMenu[counter + 2]); // back to previous menu
      }
      if (counter == 2)
      {
        display.setTextSize(1);
        display.setCursor(0, 18);
        display.print(flangerMenu[counter - 2]); // parameter
        display.setCursor(xValuePos - 18, 18);
        display.print(offsetConverted); // parameter's value
        display.setTextSize(2);
        display.setCursor(0, 27);
        display.print(flangerMenu[counter - 1]); // parameter
        display.setCursor(xValuePos - 18, 28);
        display.print(depthConverted); // parameter's value
        display.setTextSize(1);
        display.setCursor(0, 45);
        display.print(flangerMenu[counter]); // parameter
        display.setCursor(xValuePos - 18, 44);
        display.print(delayRateFloat); // parameter's value
        display.setCursor(0, 55);
        display.print(flangerMenu[counter + 1]); // back to previous menu
      }
      if (counter == 3)
      {
        display.setTextSize(1);
        display.setCursor(0, 18);
        display.print(flangerMenu[counter - 3]); // parameter
        display.setCursor(xValuePos - 18, 18);
        display.print(offsetConverted); // parameter's value
        display.setCursor(0, 27);
        display.print(flangerMenu[counter - 2]); // parameter
        display.setCursor(xValuePos - 18, 28);
        display.print(depthConverted); // parameter's value
        display.setTextSize(2);
        display.setCursor(0, 37);
        display.print("D.R."); // parameter
        display.setCursor(xValuePos - 18, 37);
        display.print(delayRateFloat); // parameter's value
        display.setTextSize(1);
        display.setCursor(0, 55);
        display.print(flangerMenu[counter]); // back to previous menu
      }
      if (counter == 4)
      {
        display.setTextSize(1);
        display.setCursor(0, 18);
        display.print(flangerMenu[counter - 4]); // parameter
        display.setCursor(xValuePos - 18, 18);
        display.print(offsetConverted); // parameter's value
        display.setCursor(0, 27);
        display.print(flangerMenu[counter - 3]); // parameter
        display.setCursor(xValuePos - 18, 27);
        display.print(depthConverted); // parameter's value
        display.setCursor(0, 37);
        display.print(flangerMenu[counter - 2]); // parameter
        display.setCursor(xValuePos - 18, 37);
        display.print(delayRateFloat); // parameter's value
        display.setTextSize(2);
        display.setCursor(0, 47);
        display.print(flangerMenu[counter - 1]); // back to previous menu
      }
      display.display();
    }
  }
  // back to previous menu
  if (SelectedMenu == 4 && counter == 4 && Encoder_Button.fell()) //
  {
    myEnc.write(1);
    counter = 1;
    SelectedMenu = 0;
  }
  // 3 edit parameters
  if (SelectedMenu == 4 && counter == 1 && Encoder_Button.rose()) // change menu so that parameter edition happens
  {
    SelectedMenu = 1041; // new menu for edition <=====
    myEnc.write(1);
    counter = 1;
  }
  if (SelectedMenu == 1041) // display and save new parameter's value
  {
    editing();
    if (RotaryEncoderEditor_I(offset, 0, 7))
    {
      offsetConverted = offset * FLANGE_DELAY_LENGTH / 4;
      flange1.voices(offsetConverted, depthConverted, delayRateFloat); // parameter change <=============
      display.setTextSize(2);
      display.setCursor(0, 33);
      display.print(flangerMenu[counter - 1]); // parameter
      display.setCursor(xValuePos - 20, 33);
      display.print(offsetConverted); // parameter's value
      display.display();
    }
    if (Edit_Button.fell()) // save parameter! <=====
    {
      // add eemprom
      EEPROM.write(eeprom_offset, offset);
      SelectedMenu = 4; // back to previous menu <====
      myEnc.write(1);
      counter = 1;
    }
  }

  if (SelectedMenu == 4 && counter == 2 && Encoder_Button.rose()) // change menu so that parameter edition happens
  {
    SelectedMenu = 1042; // new menu for edition <=====
    myEnc.write(1);
    counter = 1;
  }
  if (SelectedMenu == 1042) // display and save new parameter's value
  {
    editing();
    if (RotaryEncoderEditor_I(depth, 0, 16))
    {
      depthConverted = depth * FLANGE_DELAY_LENGTH / 4;
      flange1.voices(offsetConverted, depthConverted, delayRateFloat); // parameter change <=============
      display.setTextSize(2);
      display.setCursor(0, 33);
      display.print(flangerMenu[counter]); // parameter
      display.setCursor(xValuePos - 20, 33);
      display.print(depthConverted); // parameter's value
      display.display();
    }
    if (Edit_Button.fell()) // save parameter! <=====
    {
      // add eemprom
      EEPROM.write(eeprom_depth, depth);
      SelectedMenu = 4; // back to previous menu <====
      myEnc.write(1);
      counter = 1;
    }
  }

  if (SelectedMenu == 4 && counter == 3 && Encoder_Button.rose()) // change menu so that parameter edition happens
  {
    SelectedMenu = 1043; // new menu for edition <=====
    myEnc.write(1);
    counter = 1;
  }
  if (SelectedMenu == 1043) // display and save new parameter's value
  {
    editing();
    if (RotaryEncoderEditor_F(delayRateFloat, 0.00, 0.99)) //(delayRateConverted, 0.00, 0.99)
    {
      delayRateInt = delayRateFloat * 100;
      flange1.voices(offsetConverted, depthConverted, delayRateFloat); // parameter change <=============
      display.setTextSize(2);
      display.setCursor(0, 18);
      display.print("Delay"); // parameter
      display.setCursor(15, 33);
      display.print("Rate"); // parameter
      display.setCursor(xValuePos - 20, 33);
      display.print(delayRateFloat); // parameter's value
      display.display();
    }
    if (Edit_Button.fell()) // save parameter! <=====
    {
      // add eemprom
      EEPROM.write(eeprom_delayRate, delayRateInt);
      SelectedMenu = 4; // back to previous menu <====
      myEnc.write(1);
      counter = 1;
    }
  }

  ////////////////
  // 1 enter edit menu
  if (SelectedMenu == 0 && counter == 4 && Edit_Button.fell()) // granular
  {
    myEnc.write(1);
    counter = 0;
    editMenu(5, "Granular"); // (SelectedMenu, "display name")
  }
  // 2 display edit menu
  if (SelectedMenu == 5)
  {
    display.clearDisplay();
    display.fillRect(0, 0, 128, 16, WHITE);
    display.setTextSize(2);
    display.setTextColor(BLACK);
    display.setCursor(5, 1);
    display.print("GRANULAR");
    display.setTextColor(WHITE);

    if (RotaryEncoder(sizegranularMenu))
    {
      if (counter == 1)
      {
        display.setTextSize(2);
        display.setCursor(0, 17);
        display.print(granularMenu[counter - 1]);
        display.setCursor(xValuePos - 18, 17);
        display.print(ratio); // parameter's value
        display.setTextSize(1);
        display.setCursor(0, 33);
        display.print(granularMenu[counter]);
        display.setCursor(xValuePos - 18, 33);
        display.print(offsetConverted); // parameter's value
        display.setCursor(0, 41);
        display.print(granularMenu[counter + 1]);
        display.setCursor(xValuePos - 18, 41);
        display.print(offsetConverted); // parameter's value
        display.setCursor(0, 49);
        display.print(granularMenu[counter + 2]);
        display.setCursor(xValuePos - 18, 49);
        display.print(offsetConverted); // parameter's value
        display.setCursor(0, 57);
        display.print(granularMenu[counter + 3]);
      }

      if (counter == 2)
      {
        display.setTextSize(1);
        display.setCursor(0, 17);
        display.print(granularMenu[counter - 2]);
        display.setCursor(xValuePos - 18, 17);
        display.print(ratio); // parameter's value
        display.setTextSize(2);
        display.setCursor(0, 25);
        display.print("P.Shift");
        display.setCursor(xValuePos - 18, 25);
        display.print(offsetConverted); // parameter's value
        display.setTextSize(1);
        display.setCursor(0, 41);
        display.print(granularMenu[counter]);
        display.setCursor(xValuePos - 18, 41);
        display.print(offsetConverted); // parameter's value
        display.setCursor(0, 49);
        display.print(granularMenu[counter + 1]);
        display.setCursor(xValuePos - 18, 49);
        display.print(offsetConverted); // parameter's value
        display.setCursor(0, 57);
        display.print(granularMenu[counter + 2]);
      }

      if (counter == 3)
      {
        display.setTextSize(1);
        display.setCursor(0, 17);
        display.print(granularMenu[counter - 3]);
        display.setCursor(xValuePos - 18, 17);
        display.print(ratio); // parameter's value
        display.setCursor(0, 25);
        display.print(granularMenu[counter - 2]);
        display.setCursor(xValuePos - 18, 25);
        display.print(offsetConverted); // parameter's value
        display.setTextSize(2);
        display.setCursor(0, 33);
        display.print(granularMenu[counter - 1]);
        display.setCursor(xValuePos - 18, 33);
        display.print(offsetConverted); // parameter's value
        display.setTextSize(1);
        display.setCursor(0, 49);
        display.print(granularMenu[counter]);
        display.setCursor(xValuePos - 18, 49);
        display.print(offsetConverted); // parameter's value
        display.setCursor(0, 57);
        display.print(granularMenu[counter + 1]);
      }

      if (counter == 4) //(counter > 3 && counter < 6)
      {
        display.setTextSize(1);
        display.setCursor(0, 17);
        display.print(granularMenu[counter - 4]);
        display.setCursor(xValuePos - 18, 11);
        display.print(ratio); // parameter's value
        display.setCursor(0, 25);
        display.print(granularMenu[counter - 3]);
        display.setCursor(xValuePos - 18, 25);
        display.print(offsetConverted); // parameter's value
        display.setCursor(0, 33);
        display.print(granularMenu[counter - 2]);
        display.setCursor(xValuePos - 18, 33);
        display.print(offsetConverted); // parameter's value
        display.setTextSize(2);
        display.setCursor(0, 41);
        display.print(granularMenu[counter - 1]);
        display.setCursor(xValuePos - 18, 41);
        display.print(offsetConverted); // parameter's value
        display.setTextSize(1);
        display.setCursor(0, 57);
        display.print(granularMenu[counter]);
      }

      if (counter == 5)
      {
        display.setTextSize(1);
        display.setCursor(0, 17);
        display.print(granularMenu[counter - 5]);
        display.setCursor(xValuePos - 18, 17);
        display.print(ratio); // parameter's value
        display.setCursor(0, 25);
        display.print(granularMenu[counter - 4]);
        display.setCursor(xValuePos - 18, 25);
        display.print(offsetConverted); // parameter's value
        display.setCursor(0, 33);
        display.print(granularMenu[counter - 3]);
        display.setCursor(xValuePos - 18, 18);
        display.print(offsetConverted); // parameter's value
        display.setCursor(0, 41);
        display.print(granularMenu[counter - 2]);
        display.setCursor(xValuePos - 18, 18);
        display.print(offsetConverted); // parameter's value
        display.setTextSize(2);
        display.setCursor(0, 49);
        display.print(granularMenu[counter - 1]);
      }
      display.display();
    }
  }
  // back to previous menu
  if (SelectedMenu == 5 && counter == 5 && Encoder_Button.fell()) //
  {
    myEnc.write(1);
    counter = 1;
    SelectedMenu = 0;
  }
  // 3 edit parameters
  if (SelectedMenu == 5 && counter == 1 && Encoder_Button.rose()) // change menu so that parameter edition happens
  {
    SelectedMenu = 1051; // new menu for edition <=====
    myEnc.write(1);
    counter = 1;
  }
  if (SelectedMenu == 1051) // display and save new parameter's value
  {
    editing();
    if (RotaryEncoderEditor_F(ratio, 0.12, 8.0))
    {
      ratioInt = ratio * 100;
      granular1.setSpeed(ratio); // parameter change <=============
      display.setTextSize(2);
      display.setCursor(0, 20);
      display.print(granularMenu[counter]); // parameter
      display.setCursor(xValuePos - 20, 40);
      display.print(ratio); // parameter's value
      display.display();
    }
    if (Edit_Button.fell()) // save parameter! <=====
    {
      // add eemprom
      EEPROM.write(eeprom_ratio, ratioInt);
      SelectedMenu = 5; // back to previous menu <====
      myEnc.write(1);
      counter = 1;
    }
  }

  if (SelectedMenu == 5 && counter == 2 && Encoder_Button.rose()) // change menu so that parameter edition happens
  {
    SelectedMenu = 1052; // new menu for edition <=====
    myEnc.write(1);
    counter = 1;
  }
  if (SelectedMenu == 1052) // display and save new parameter's value
  {
    editing();
    granular1.beginPitchShift(grainLenght); // parameter change <=============
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print("Pitch"); // parameter
    display.setCursor(15, 40);
    display.print("Shift"); // parameter
    if (RotaryEncoder(digitsMenu))
    {
      if (counter == 1 && digitsEditCounter == 0)
      {
        display.setCursor(82, 40);
        display.print(grainLenght); // parameter's value
        display.setCursor(82, 56);
        display.print("^");
        display.display();
      }
      if (counter == 2 && digitsEditCounter == 0)
      {
        display.setCursor(82, 40);
        display.print(grainLenght); // parameter's value
        display.setCursor(94, 56);
        display.print("^");
        display.display();
      }
      if (counter == 3 && digitsEditCounter == 0)
      {
        display.setCursor(82, 40);
        display.print(grainLenght); // parameter's value
        display.setCursor(106, 56);
        display.print("^");
        display.display();
      }
      if (counter == 4 && digitsEditCounter == 0)
      {
        display.setCursor(82, 40);
        display.print(grainLenght); // parameter's value
        display.setCursor(118, 56);
        display.print("^");
        display.display();
      }
    }
    if (counter == 1 && Encoder_Button.fell())
    {
      SelectedMenu = 10521;
    }
    if (counter == 2 && Encoder_Button.fell())
    {
      SelectedMenu = 10522;
    }
    if (counter == 3 && Encoder_Button.fell())
    {
      SelectedMenu = 10523;
    }
    if (counter == 4 && Encoder_Button.fell())
    {
      SelectedMenu = 10524;
    }
  }

  if (SelectedMenu == 10521)
  {
    editing();
    granular1.beginPitchShift(grainLenght); // parameter change <=============
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print("Pitch"); // parameter
    display.setCursor(15, 40);
    display.print("Shift"); // parameter
    if (RotaryEncoder(numberSelectionMenu))
    {
      thousands = (counter - 1)*(1000);
      desiredNumber = ones + tens + hundreds + thousands;
      grainLenght = desiredNumber;
      display.setCursor(82, 40); //82 - 94 - 106 - 118
      display.print(thousands); // parameter's value in String
      display.setCursor(94, 40); //82 - 94 - 106 - 118
      display.print(hundreds); // parameter's value in String
      display.setCursor(106, 40); //82 - 94 - 106 - 118
      display.print(tens); // parameter's value in String
      display.setCursor(118, 40); //82 - 94 - 106 - 118
      display.print(ones); // parameter's value in String
      display.setCursor(82, 56);
      display.print("^");
      display.display();
    }
    if (Edit_Button.fell())
    {
      myEnc.write(1);
      counter = 1;
      SelectedMenu = 1052; // back to previous menu <====
    }
  }

  if (SelectedMenu == 10522)
  {
    editing();
    granular1.beginPitchShift(grainLenght); // parameter change <=============
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print("Pitch"); // parameter
    display.setCursor(15, 40);
    display.print("Shift"); // parameter
    if (RotaryEncoder(numberSelectionMenu))
    {
      hundreds = (counter - 1)*(100);
      desiredNumber = ones + tens + hundreds + thousands;
      grainLenght = desiredNumber;
      display.setCursor(82, 40); //82 - 94 - 106 - 118
      display.print(thousands); // parameter's value in String
      display.setCursor(94, 40); //82 - 94 - 106 - 118
      display.print(hundreds); // parameter's value in String
      display.setCursor(106, 40); //82 - 94 - 106 - 118
      display.print(tens); // parameter's value in String
      display.setCursor(118, 40); //82 - 94 - 106 - 118
      display.print(ones); // parameter's value in String
      display.setCursor(94, 56);
      display.print("^");
      display.display();
    }
    if (Edit_Button.fell())
    {
      myEnc.write(1);
      counter = 1;
      SelectedMenu = 1052; // back to previous menu <====
    }
  }
  if (SelectedMenu == 10523)
  {
    editing();
    granular1.beginPitchShift(grainLenght); // parameter change <=============
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print("Pitch"); // parameter
    display.setCursor(15, 40);
    display.print("Shift"); // parameter
    if (RotaryEncoder(numberSelectionMenu))
    {
      tens = (counter - 1)*(10);
      desiredNumber = ones + tens + hundreds + thousands;
      grainLenght = desiredNumber;
      display.setCursor(82, 40); //82 - 94 - 106 - 118
      display.print(thousands); // parameter's value in String
      display.setCursor(94, 40); //82 - 94 - 106 - 118
      display.print(hundreds); // parameter's value in String
      display.setCursor(106, 40); //82 - 94 - 106 - 118
      display.print(tens); // parameter's value in String
      display.setCursor(118, 40); //82 - 94 - 106 - 118
      display.print(ones); // parameter's value in String
      display.setCursor(106, 56);
      display.print("^");
      display.display();
    }
    if (Edit_Button.fell())
    {
      myEnc.write(1);
      counter = 1;
      SelectedMenu = 1052; // back to previous menu <====
    }
  }
  if (SelectedMenu == 10524)
  {
    editing();
    granular1.beginPitchShift(grainLenght); // parameter change <=============
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print("Pitch"); // parameter
    display.setCursor(15, 40);
    display.print("Shift"); // parameter
    if (RotaryEncoder(numberSelectionMenu))
    {
      ones = counter - 1;
      desiredNumber = ones + tens + hundreds + thousands;
      grainLenght = desiredNumber;
      display.setCursor(82, 40); //82 - 94 - 106 - 118
      display.print(thousands); // parameter's value in String
      display.setCursor(94, 40); //82 - 94 - 106 - 118
      display.print(hundreds); // parameter's value in String
      display.setCursor(106, 40); //82 - 94 - 106 - 118
      display.print(tens); // parameter's value in String
      display.setCursor(118, 40); //82 - 94 - 106 - 118
      display.print(ones); // parameter's value in String
      display.setCursor(118, 56);
      display.print("^");
      display.display();
    }
    if (Edit_Button.fell())
    {
      myEnc.write(1);
      counter = 1;
      SelectedMenu = 1052; // back to previous menu <====
    }
  }

  /*if (RotaryEncoderEditor_I(grainLenght, -1, GRANULAR_MEMORY_SIZE - 1))
  {
  }
  if (Edit_Button.fell()) // save parameter! <=====
  {
    // add eemprom
    EEPROM.write(eeprom_grainLenght, grainLenght);
    SelectedMenu = 5; // back to previous menu <====
    myEnc.write(1);
    counter = 1;
  }
  */

  if (SelectedMenu == 5 && counter == 3 && Encoder_Button.rose()) // change menu so that parameter edition happens
  {
    SelectedMenu = 1053; // new menu for edition <=====
    myEnc.write(1);
    counter = 1;
  }

  ////////////

  // 1 enter edit menu
  if (SelectedMenu == 0 && counter == 5 && Edit_Button.fell()) // Reverb
  {
    myEnc.write(1);
    counter = 0;
    editMenu(6, "Reverb"); // (SelectedMenu, "display name")
  }
  // 2 display edit menu
  if (SelectedMenu == 6)
  {
    display.clearDisplay();
    display.fillRect(0, 0, 128, 16, WHITE);
    display.setTextSize(2);
    display.setTextColor(BLACK);
    display.setCursor(5, 1);
    display.print("REVERB");
    display.setTextColor(WHITE);

    if (RotaryEncoder(sizefreeverbMenu))
    {
      if (counter == 1)
      {
        display.setTextSize(2);
        display.setCursor(0, 17);
        display.print("R.Size"); // parameter
        display.setCursor(xValuePos - 20, 17);
        display.print(roomsizeFloat); // parameter's value
        display.setTextSize(1);
        display.setCursor(0, 38);
        display.print(freeverbMenu[counter]); // parameter
        display.setCursor(xValuePos - 20, 38);
        display.print(dampingFloat); // parameter's value
        display.setTextSize(1);
        display.setCursor(0, 54);
        display.print(freeverbMenu[counter + 1]); // parameter
      }
      if (counter == 2)
      {
        display.setTextSize(1);
        display.setCursor(0, 17);
        display.print(freeverbMenu[counter - 2]); // parameter
        display.setCursor(xValuePos - 20, 17);
        display.print(roomsizeFloat); // parameter's value
        display.setTextSize(2);
        display.setCursor(0, 30);
        display.print("Damp."); // parameter
        display.setCursor(xValuePos - 20, 30);
        display.print(dampingFloat); // parameter's value
        display.setTextSize(1);
        display.setCursor(0, 54);
        display.print(freeverbMenu[counter]); // parameter
      }
      if (counter == 3)
      {
        display.setTextSize(1);
        display.setCursor(0, 17);
        display.print(freeverbMenu[counter - 3]); // parameter
        display.setCursor(xValuePos - 20, 17);
        display.print(roomsizeFloat); // parameter's value
        display.setCursor(0, 30);
        display.print(freeverbMenu[counter - 2]); // parameter
        display.setCursor(xValuePos - 20, 30);
        display.print(dampingFloat); // parameter's value
        display.setTextSize(2);
        display.setCursor(0, 46);
        display.print(freeverbMenu[counter - 1]); // parameter
      }
      display.display();
    }
  }
  // back to previous menu
  if (SelectedMenu == 6 && counter == 3 && Encoder_Button.fell()) //
  {
    myEnc.write(1);
    counter = 1;
    SelectedMenu = 0;
  }
  // 3 edit parameters
  if (SelectedMenu == 6 && counter == 1 && Encoder_Button.rose()) // change menu so that parameter edition happens
  {
    SelectedMenu = 1061; // new menu for edition <=====
    myEnc.write(1);
    counter = 1;
  }
  if (SelectedMenu == 1061) // display and save new parameter's value
  {
    editing();
    if (RotaryEncoderEditor_F(roomsizeFloat, -0.01, .99))
    {
      roomsizeInt = roomsizeFloat * 100;
      freeverb1.roomsize(roomsizeFloat); // parameter change <=============
      display.setTextSize(2);
      display.setCursor(0, 20);
      display.print(freeverbMenu[counter]); // parameter
      display.setCursor(xValuePos - 20, 40);
      display.print(roomsizeFloat); // parameter's value
      display.display();
    }
    if (Edit_Button.fell()) // save parameter! <=====
    {
      // add eemprom
      EEPROM.write(eeprom_roomsize, roomsizeInt);
      SelectedMenu = 6; // back to previous menu <====
      myEnc.write(1);
      counter = 1;
    }
  }

  if (SelectedMenu == 6 && counter == 2 && Encoder_Button.rose()) // change menu so that parameter edition happens
  {
    SelectedMenu = 1062; // new menu for edition <=====
    myEnc.write(1);
    counter = 1;
  }
  if (SelectedMenu == 1062) // display and save new parameter's value
  {
    editing();
    if (RotaryEncoderEditor_F(dampingFloat, -0.01, .99))
    {
      dampingInt = dampingFloat * 100;
      freeverb1.damping(dampingFloat); // parameter change <=============
      display.setTextSize(2);
      display.setCursor(0, 20);
      display.print(freeverbMenu[counter]); // parameter
      display.setCursor(xValuePos - 20, 40);
      display.print(dampingFloat); // parameter's value
      display.display();
    }
    if (Edit_Button.fell()) // save parameter! <=====
    {
      // add eemprom
      EEPROM.write(eeprom_damping, dampingInt);
      SelectedMenu = 6; // back to previous menu <====
      myEnc.write(1);
      counter = 1;
    }
  }

  if (SelectedMenu == 6 && counter == 3 && Encoder_Button.rose()) // change menu so that parameter edition happens
  {
    SelectedMenu = 1063; // new menu for edition <=====
    myEnc.write(1);
    counter = 1;
  }

  ///////////
}

/*
  if (enterMenu == false && menubut == false){
  editionInProgress = true;
  Serial.println("editing");
  effectpatchCord.disconnect();
  } else {
    effectpatchCord.connect();
  };
  */

//==================================================//
//==================  FUNCTIONS  ===================//
//==================================================//

/////////// map Float numbers
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/////////// convert float to int by multiplying * 100
int floatToIntX_00(float val)
{
  val = val * 100;
  return val;
}

/////////// convert int to float by multiplying * 0.01
float intToFloatX_00(int val)
{
  float F_val;
  F_val = val * .01;
  return F_val;
}
/////////// convert float to int by multiplying * 10
int floatToIntX_0(float val)
{
  val = val * 10;
  return val;
}

/////////// convert int to float by multiplying * 0.1
float intToFloatX_0(int val)
{
  float F_val;
  F_val = val * .1;
  return F_val;
}

bool RotaryEncoderEditor_I(int &currentParam, int minValue, int maxValue)
{
  bool change = false;
  long newPosition = (myEnc.read() / 4);
  int counter = currentParam;

  if (newPosition != oldPosition)
  {
    if (newPosition > oldPosition)
    {
      counter++;
    }
    else
    {
      counter--;
    }

    if (counter <= minValue + 1)
    {
      counter = minValue + 1;
    }
    if (counter >= maxValue + 1)
    {
      counter = maxValue + 1;
    }
    // Serial.print("counter int in function: ");
    // Serial.println(counter);
    oldPosition = newPosition;
    change = true;
    currentParam = counter;
  }
  return change;

  /*
    //IMPLEMENTATION IN MAIN LOOP
  if (RotaryEncoderEditor_I(thiisaninttest, -5, 8)){
  Serial.println(thiisaninttest-1);
    }
  */
}

bool RotaryEncoderEditor_F(float &currentParamF, float minValueF, float maxValueF)
{
  bool change = false;
  long newPosition = (myEnc.read() / 4);
  float counterf = currentParamF;

  if (newPosition != oldPosition)
  {
    if (newPosition > oldPosition)
    {
      counterf = counterf + 0.01;
    }
    else
    {
      counterf = counterf - 0.01;
    }

    if (counterf <= minValueF + 0.01)
    {
      counterf = minValueF + 0.01;
    }
    if (counterf >= maxValueF + 0.01)
    {
      counterf = maxValueF + 0.01;
    }
    // Serial.print("counter int in function: ");
    // Serial.println(counter);
    oldPosition = newPosition;
    change = true;
    currentParamF = counterf;
  }
  return change;
  /*
    //IMPLEMENTATION IN MAIN LOOP
    if (RotaryEncoderEditor_F(thiisafloattest, -.5, .8))// 2
    {
    // thiisafloattest = (thiisafloattest-1)* 0.01;
      Serial.println(thiisafloattest-0.01);
    }
     */
}