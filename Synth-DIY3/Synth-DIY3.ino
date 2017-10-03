// Demo the quad alphanumeric display LED backpack kit
// scrolls through every character, then scrolls Serial
// input onto the display

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_AlphaNum4 alpha = Adafruit_AlphaNum4();

void setup() {
	Serial.begin(9600);

	alpha.begin(0x70);  // pass in the address
	/*
	alpha.writeDigitRaw(3, 0x0);
	alpha.writeDigitRaw(0, 0xFFFF);
	alpha.writeDisplay();
	delay(200);
	alpha.writeDigitRaw(0, 0x0);
	alpha.writeDigitRaw(1, 0xFFFF);
	alpha.writeDisplay();
	delay(200);
	alpha.writeDigitRaw(1, 0x0);
	alpha.writeDigitRaw(2, 0xFFFF);
	alpha.writeDisplay();
	delay(200);
	alpha.writeDigitRaw(2, 0x0);
	alpha.writeDigitRaw(3, 0xFFFF);
	alpha.writeDisplay();
	delay(200);

	alpha.clear();
	alpha.writeDisplay();

	// display every character, 
	for (uint8_t i = '!'; i <= 'z'; i++) {
		alpha.writeDigitAscii(0, i);
		alpha.writeDigitAscii(1, i + 1);
		alpha.writeDigitAscii(2, i + 2);
		alpha.writeDigitAscii(3, i + 3);
		alpha.writeDisplay();

		delay(300);
	}
	*/
	Serial.println("Start typing to display!!!!!!");
}


char displaybuffer[4] = { ' ', ' ', ' ', ' ' };

void loop() {
	while (!Serial.available()) return;

	char c = Serial.read();
	if (!isprint(c)) return; // only printable!

							 // scroll down display
	displaybuffer[0] = displaybuffer[1];
	displaybuffer[1] = displaybuffer[2];
	displaybuffer[2] = displaybuffer[3];
	displaybuffer[3] = c;

	// set every digit to the buffer
	alpha.writeDigitAscii(0, displaybuffer[0]);
	alpha.writeDigitAscii(1, displaybuffer[1]);
	alpha.writeDigitAscii(2, displaybuffer[2]);
	alpha.writeDigitAscii(3, displaybuffer[3]);

	// write it out!
	alpha.writeDisplay();
	delay(200);
}






/*
#include "Arduino.h"
#include "Workstation.h"

void setup()
{
	Serial.begin(115200);
	Serial.println("Hello Workstation");
	initHardware();

}

void loop()
{

	run();

}
*/