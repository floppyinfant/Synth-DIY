#include "DeviceLED.h"

DeviceLED::DeviceLED()
{
}

DeviceLED::DeviceLED(int pin)
{
	ledType = Ledtype::SINGLE;
	ledPin = pin;
	ledState = LOW;
	brighness = 1025;
	previousMillis = millis();
}

DeviceLED::DeviceLED(int pin_r, int pin_g, int pin_b)
{
	ledType = Ledtype::RGB;
	redPin = pin_r;
	greenPin = pin_g;
	bluePin = pin_b;
	ledState = LOW;
	brighness = 1025;
	previousMillis = millis();
}


DeviceLED::~DeviceLED()
{
}

// ----------------------------------------------------------------------------

void DeviceLED::begin()
{
	// TODO set initial values
	// use with update()
	// @see Adafruit Libraries
}

void DeviceLED::update()
{
	currentMillis = millis();
	if (currentMillis - previousMillis >= interval) {
		if (ledType == Ledtype::SINGLE) {
			digitalWrite(ledPin, ledState);
		}
		else if (ledType == Ledtype::RGB) {
			analogWrite(redPin, red);
			analogWrite(greenPin, green);
			analogWrite(bluePin, blue);
		}
	}
}


void DeviceLED::blink(long interval) {

	currentMillis = millis();
	if (currentMillis - previousMillis >= interval) {
		// save the last time you blinked the LED 
		previousMillis = currentMillis;

		// if the LED is off turn it on and vice-versa:
		if (ledState == LOW)
			ledState = HIGH;
		else
			ledState = LOW;

		// set the LED with the ledState of the variable:
		digitalWrite(ledPin, ledState);
	}
}

void DeviceLED::fade(int from, int to, enum Direction, long time)
{
	/*
	// TODO
	if (brighness > 0) {

	}

	// ...

	if (ledType == Ledtype::SINGLE) {
		
	} else if (ledType == Ledtype::RGB) {

	}
	*/
}

void DeviceLED::setRGB(int r, int g, int b)
{
	red = r;
	green = g;
	blue = b;

	analogWrite(redPin, r);
	analogWrite(greenPin, g);
	analogWrite(bluePin, b);
}
