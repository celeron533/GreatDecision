/*
  Universal 8bit Graphics Library, http://code.google.com/p/u8glib/
  */

#include "HID.h"
#include "Keyboard.h"
#include "U8glib.h"


enum LOOP_STATUS
{
	LOOP_STANDBY,
	LOOP_PRESSED,
	LOOP_MENU
} LoopStatus;


enum MENU_STATUS
{
	MENU_SHORTCUT,
	MENU_MAIN
} MenuStatus;

#pragma region ScreenSetup

//Pins for screen SPI
#define SPI_CS U8G_PIN_NONE	//ChipSelect is already connected by the screen manufacture
#define SPI_A0 9
#define SPI_RST 8
// Init the screen hardware
// HW SPI Com: CS = N/A, A0 = 9, RST = 8 (Arduino pins are SCK = 15 and MOSI = 16)
/* Physical connection: screen -- board
* GND -- GND
* VCC -- VCC
* SCL -- 15 (SCK)
* SDA -- 16 (MOSI)
* RST -- 8 (RST, pin configurable)
* D/C -- 9 (A0, pin configurable)
*/
U8GLIB_SSD1306_128X64 u8g(SPI_CS, SPI_A0, SPI_RST);

void ScreenSetup()
{
	//rotate screen, if required
	//u8g.setRot180();
	u8g.setColorIndex(1);
}

#pragma endregion

#pragma region KeyboardSetup

//Used to statistic key press. Currently only support one key down event.
enum KEYCODE
{
	KEYCODE_NONE,
	KEYCODE_UP,
	KEYCODE_DOWN,
	KEYCODE_SELECT,
	KEYCODE_DECISION,
	KEYCODE_BACK
};

typedef struct KeyCodeStruct
{
	enum KEYCODE keyCodeFirst;
	enum KEYCODE keyCodeSecond;
	enum KEYCODE keyCode;
	uint8_t debounceCount;
	bool isChanged;
};
KeyCodeStruct KeyCode;

//Hardware: Keyboard --> Pin
#define PIN_KEY_UP 2
#define PIN_KEY_DOWN 3
#define PIN_KEY_SELECT 4
#define PIN_KEY_BACK 5
#define PIN_KEY_DECISION 6

//setup keyboard mapping
void KeyboardSetup() {
	// configure input keys, default is HIGH, when pressed, turns LOW
	pinMode(PIN_KEY_UP, INPUT);
	digitalWrite(PIN_KEY_UP, HIGH);

	pinMode(PIN_KEY_DOWN, INPUT);
	digitalWrite(PIN_KEY_DOWN, HIGH);

	pinMode(PIN_KEY_SELECT, INPUT);
	digitalWrite(PIN_KEY_SELECT, HIGH);

	pinMode(PIN_KEY_BACK, INPUT);
	digitalWrite(PIN_KEY_BACK, HIGH);

	pinMode(PIN_KEY_DECISION, INPUT);
	digitalWrite(PIN_KEY_DECISION, HIGH);

	KeyCode = { KEYCODE_NONE, KEYCODE_NONE, KEYCODE_NONE, 0, false };
}

#define DEBOUNCE_COUNT 10

void ListeningKey() {
	//backup the _KeyCodeFirst to _KeyCodeSecond
	KeyCode.keyCodeSecond = KeyCode.keyCodeFirst;
	//read the key to keyCodeFirst
	if (digitalRead(PIN_KEY_UP) == LOW)
		KeyCode.keyCodeFirst = KEYCODE_UP;
	else if (digitalRead(PIN_KEY_DOWN) == LOW)
		KeyCode.keyCodeFirst = KEYCODE_DOWN;
	else if (digitalRead(PIN_KEY_SELECT) == LOW)
		KeyCode.keyCodeFirst = KEYCODE_SELECT;
	else if (digitalRead(PIN_KEY_BACK) == LOW)
		KeyCode.keyCodeFirst = KEYCODE_BACK;
	else if (digitalRead(PIN_KEY_DECISION) == LOW)
		KeyCode.keyCodeFirst = KEYCODE_DECISION;
	else
		KeyCode.keyCodeFirst = KEYCODE_NONE;

	//Debounce algorithm. If the keyCode is stable for pre-defined counts, commit the keyCode.
	//It is better to add resister and resister on the board.
	KeyCode.isChanged = false;
	if (KeyCode.keyCodeSecond == KeyCode.keyCodeFirst)
	{
		if (KeyCode.debounceCount < DEBOUNCE_COUNT)
			KeyCode.debounceCount++;
		else
		{
			KeyCode.keyCode = KeyCode.keyCodeFirst;
			KeyCode.isChanged = true;
		}
	}
	else
		KeyCode.debounceCount = 0;
}

#pragma endregion

void setup()
{
	KeyboardSetup();
	ScreenSetup();
}


void loop()
{
	ListeningKey();	//listen the key press event, result is stored in [KeyCode]

	//todo: key logic

	//determine the loop status
	switch (LoopStatus)
	{
	case LOOP_STANDBY:
		// do something
		break;

	case LOOP_PRESSED:
		// do something
		break;

	case LOOP_MENU:
		loop_menu();
		break;

	default:
		// do something
		;
	}
}

void loop_menu()
{
	switch (MenuStatus)
	{
	case MENU_SHORTCUT:
		// do something
		break;
	case MENU_MAIN:
		// do something
		break;
	default:
		// do something
		;
	}
}

