/*
Button-LED
----------


RGB-LED
-------
Example code: Arduino communication ReadASCIIString.ino
...
// constrain the values to 0 - 255 and invert
// if you're using a common-cathode LED, just use "constrain(color, 0, 255);"
red = 255 - constrain(red, 0, 255);
...
// fade the red, green, and blue legs of the LED:
analogWrite(redPin, red);
...

*/

#ifndef DEVICE_LED_H
#define DEVICE_LED_H

#include "Arduino.h"

enum Ledtype {SINGLE, RGB};
enum Direction {UP, DOWN, UPANDDOWN};

class DeviceLED
{
public:
	DeviceLED();
	DeviceLED(int);
	DeviceLED(int, int, int);
	~DeviceLED();

	void begin();        // set initial values
	void update(void);

	void blink(long);
	void fade(int, int, enum Direction, long);

	void setRGB(int, int, int);

private:
	enum Ledtype ledType;

	int ledPin;
	int redPin;
	int greenPin;
	int bluePin;

	int brighness;       // 8-bit, 255 values
	int ledState;        // LOW || HIGH

	int red;             // 8-bit, 255 values
	int green;
	int blue;

	long currentMillis;
	long previousMillis;
	int interval;        // [ms]
};

#endif // DEVICE_LED_H
