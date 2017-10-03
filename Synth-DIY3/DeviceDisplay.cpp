#include "DeviceDisplay.h"

DeviceDisplay::DeviceDisplay() :
	alpha4()
{
	// Declaration
	//Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

	alpha4.begin(ADDR_ALPHA);
}

DeviceDisplay::~DeviceDisplay()
{
}

/* TODO: This is Blocking Code ! */
void DeviceDisplay::alphaWrite(char *text) {

	static char displaybuffer[4] = { ' ', ' ', ' ', ' ' };

	for (int i = 0; i<sizeof(text); i++) {
		char c = text[i];

		// scroll down display
		displaybuffer[0] = displaybuffer[1];
		displaybuffer[1] = displaybuffer[2];
		displaybuffer[2] = displaybuffer[3];
		displaybuffer[3] = c;

		// set every digit to the buffer
		alpha4.writeDigitAscii(0, displaybuffer[0]);
		alpha4.writeDigitAscii(1, displaybuffer[1]);
		alpha4.writeDigitAscii(2, displaybuffer[2]);
		alpha4.writeDigitAscii(3, displaybuffer[3]);

		// write it out!
		alpha4.writeDisplay();
		delay(200);
	}
}

/* Demo */
/* TODO: This is Blocking Code ! */
void DeviceDisplay::alphaReadSerial() {
	char displaybuffer[4] = { ' ', ' ', ' ', ' ' };

	while (!Serial.available()) return;

	char c = Serial.read();
	if (!isprint(c)) return; // only printable!

							 // scroll down display
	displaybuffer[0] = displaybuffer[1];
	displaybuffer[1] = displaybuffer[2];
	displaybuffer[2] = displaybuffer[3];
	displaybuffer[3] = c;

	// set every digit to the buffer
	alpha4.writeDigitAscii(0, displaybuffer[0]);
	alpha4.writeDigitAscii(1, displaybuffer[1]);
	alpha4.writeDigitAscii(2, displaybuffer[2]);
	alpha4.writeDigitAscii(3, displaybuffer[3]);

	// write it out!
	alpha4.writeDisplay();
	delay(200);
}
