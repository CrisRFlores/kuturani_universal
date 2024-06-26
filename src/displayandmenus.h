#include <Wire.h> // SCL pin 19, SDA pin 18
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels (usually 128)
#define SCREEN_HEIGHT 64 // OLED display height, in pixels (64 for larger oLEDs)
#define OLED_RESET -1	 // Reset pin gpio (or -1 if sharing Arduino reset pin)

int test = 45;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1, 1000000); // 1MHz I2C clock

// '9949', 128x64px
const unsigned char epd_bitmap_9949[] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 1040)
const int epd_bitmap_allArray_LEN = 1;
const unsigned char *epd_bitmap_allArray[1] = {
	epd_bitmap_9949};

void testdrawbitmap(void)
{
	display.clearDisplay();
	display.drawBitmap(
		(display.width() - 128) / 2,
		(display.height() - 64) / 2,
		epd_bitmap_9949, 128, 64, 1);
	display.display();
	delay(1000);
}

//===================== MENU STUFF ==========================
//===================== MENU STUFF ==========================
//===================== MENU STUFF ==========================
//===================== MENU STUFF ==========================

// menu system variables
int SelectedPreset = 0;

// menu levels
int SelectedMenu = 0;

// save current encoder selection
String currentEncoderSelection = {"current"};

// selection indicator
String selectionIndicator[] = {">", "<<<<<<<<<<<<<<<"};

// mark if custom menu has been approved
bool customMenu_activated = false;

// vertical position of value being edited
int xValuePos = 100;

// menu variables
String MainMenuConst[] = { // Main Menu - SelectedMenu = 0 ------ PROGMEM
	"Bitcrusher",
	"Chorus",
	"Flanger",
	"Granular",
	"Reverb",
	"Custom"};

String MainMenu[] = { // Main Menu - SelectedMenu = 0
	"Bitcrusher",
	"Chorus",
	"Flanger",
	"Granular",
	"Reverb",
	"Custom"};
int sizeMainMenu = sizeof(MainMenu) / sizeof(MainMenu[0]);

String customMenuConst[] = { // Main Menu - SelectedMenu = 0 ---- PROGMEM
	"Custom 1",
	"Custom 2",
	"Custom 3",
	"Custom 4",
	"Custom 5",
	"Custom 6",
	"Custom 7",
	"Custom 8",
	"Custom 9",
	"Custom 10",
	"Custom 11",
	"Custom 12",
	"Custom 13",
	"Custom 14",
	"Custom 15",
	"Custom 16",
	"Custom 17",
	"Custom 18",
	"Custom 19",
	"Custom 20",
	"Custom 21",
	"Custom 22",
	"Custom 23",
	"Custom 24",
	"Custom 25",
	"<- BACK"};

String customMenu[] = { // custom menu
	"Custom 1",
	"Custom 2",
	"Custom 3",
	"Custom 4",
	"Custom 5",
	"Custom 6",
	"Custom 7",
	"Custom 8",
	"Custom 9",
	"Custom 10",
	"Custom 11",
	"Custom 12",
	"Custom 13",
	"Custom 14",
	"Custom 15",
	"Custom 16",
	"Custom 17",
	"Custom 18",
	"Custom 19",
	"Custom 20",
	"Custom 21",
	"Custom 22",
	"Custom 23",
	"Custom 24",
	"Custom 25",
	"<- BACK"};
int sizecustomMenu = sizeof(customMenu) / sizeof(customMenu[0]);

// SelectedMenu = 2
String bitcrusherMenu[] = { // bitcrusher
	"Bits",
	"Sample Rate",
	"<- BACK"};
int sizebitcrusherMenu = sizeof(bitcrusherMenu) / sizeof(bitcrusherMenu[0]);

String chorusMenu[] = { // chorus
	/*"Begin",*/
	"Voices",
	"<- BACK"};
int sizechorusMenu = sizeof(chorusMenu) / sizeof(chorusMenu[0]);

String flangerMenu[] = { // flanger
	//"Voices...",
	"Offset",
	"Depth",
	"Delay Rate",
	"<- BACK"};
int sizeflangerMenu = sizeof(flangerMenu) / sizeof(flangerMenu[0]);

String granularMenu[] = {
	// granular
	"Speed",	   // set/manual   "Set Speed",
	"Pitch Shift", // set,manual, --no begin freeze--  "Begin Pitch Shift",
	"Begin",	   // auto/manual  "Begin Freeze",
	"Stop",		   // auto/manual  "Stop Freeze"
	"<- BACK"};
int sizegranularMenu = sizeof(granularMenu) / sizeof(granularMenu[0]);

String freeverbMenu[] = { // freeverb
	"Roomsize",			  // 0. - 1.
	"Damping",
	"<- BACK"}; // 0. - 1.
int sizefreeverbMenu = sizeof(freeverbMenu) / sizeof(freeverbMenu[0]);

//=========== make a big number using 1s, 10ths, 100ths, 1000ths
int ones = 0;
int tens = 0;
int hundreds = 0;
int thousands = 0;
String onesStr;
String tensStr;
String hundredsStr;
String thousandsStr;

int digits[] = {0, 1, 2, 3};
int digitsMenu = sizeof(digits) / sizeof(digits[0]);

int numberSelection[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
String numberSelectionDisplay[] = {
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

int numberSelectionMenu = sizeof(numberSelection) / sizeof(numberSelection[0]);

int desiredNumber = 0;
String desiredNumberString;

int digitsEditCounter = 0;
//========== make a big number using 1s, 10ths, 100ths, 1000ths

void DisplayInMainLoop(void)
{
	display.clearDisplay();		// clear buffer
	for (int r = 0; r < 4; r++) // 4 es el numero de lineas que caben si el texto es 2
	{
		for (int y = 0; y < 4; y++) // fill buffer completely withs chars
		{
			display.setTextSize(2); // tamano del texto
			display.setTextColor(1);
			display.setCursor(0, (y * 16));
			display.print(test);
			display.print(" MAYA");
		}
		display.setTextSize(2); // erase one line at the time
		display.setTextColor(0);
		display.setCursor(0, (r * 16));
		display.print(test);
		display.print(" MAYA");

		display.display();
		// delay(1000);
	}
}

//////////////////////////////////////////////////

// this is an example of the code to mark selected presets
/*
 if (SelectedMenu == 0 && counter == 1 && Button1.fell()) // bitcrusher
  {
	currentEncoderSelection[0] = MainMenu[0];
	for (int i = 0; i < 6; i++)
	{
	  MainMenu[i] = MainMenuConst[i];
	}

	MainMenu[0] = selectionIndicator[0] + currentEncoderSelection[0];
	// SelectedMenu = 1;
	// SelectedMenu = 0;
	// aver = counter;
	// counter = counter + 1;
	// counter = aver;

	display.clearDisplay();
	display.setTextSize(2);
	display.setCursor(0, 25);
	display.print(currentEncoderSelection[0]);
	display.display();
  }
*/

void chosenPresetMainMenu(int selection)
{
	currentEncoderSelection = MainMenu[selection - 1];
	for (int i = 0; i < 6; i++) // reset (remove ">") mainmenu names
	{
		MainMenu[i] = MainMenuConst[i];
	}
	for (int i = 0; i < 25; i++) // reset (remove ">") custommenu names
	{
		customMenu[i] = customMenuConst[i];
	}
	MainMenu[selection - 1] = selectionIndicator[0] + currentEncoderSelection + selectionIndicator[1];
	display.clearDisplay();
	display.setTextSize(2);
	display.setCursor(0, 25);
	display.print(currentEncoderSelection);
	display.display();
}

void chosenPresetCustomMenu(int selection)
{
	currentEncoderSelection = customMenu[selection - 1];
	for (int i = 0; i < 6; i++) // reset (remove ">") mainmenu names
	{
		MainMenu[i] = MainMenuConst[i];
	}
	for (int i = 0; i < 25; i++) // reset (remove ">") custommenu names
	{
		customMenu[i] = customMenuConst[i];
	}
	customMenu[selection - 1] = selectionIndicator[0] + currentEncoderSelection + selectionIndicator[1];
	display.clearDisplay();
	display.setTextSize(2);
	display.setCursor(0, 25);
	display.print(currentEncoderSelection);
	display.display();
	MainMenu[5] = {">Custom<<<<<<<<<<<<<<<<"}; // mark main menu
}

void editMenu(int selected, String selection) // (SelectedMenu, "display name")
{
	SelectedMenu = selected;
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(45, 18);
	display.print("EDITING");
	display.setTextSize(2);
	display.setCursor(5, 33);
	display.print(selection);
	display.display();
}

void editing(void)
{
	display.clearDisplay();
	display.fillRect(0, 0, 128, 16, WHITE);
	display.setTextSize(2);
	display.setTextColor(BLACK);
	display.setCursor(5, 1);
	display.print("Editing...");
	display.setTextColor(WHITE);
};

//===================== AUDIO STUFF ==========================
//===================== AUDIO STUFF ==========================
//===================== AUDIO STUFF ==========================
//===================== AUDIO STUFF ==========================
