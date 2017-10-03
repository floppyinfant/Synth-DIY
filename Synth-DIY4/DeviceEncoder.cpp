/** Rotary-Encoder
* Encoder + Pushbutton + RGB-LED
* http://playground.arduino.cc/Main/RotaryEncoders
*
* https://www.circuitsathome.com/mcu/programming/reading-rotary-encoder-on-arduino
* http://www.circuitsathome.com/mcu/rotary-encoder-interrupt-service-routine-for-avr-micros
* https://code.google.com/p/m2tklib/wiki/t09
*/

#include "DeviceEncoder.h"

DeviceEncoder::DeviceEncoder()
{
}

DeviceEncoder::~DeviceEncoder()
{
}

/**
* Read Encoder Change
* @see https://de.wikipedia.org/wiki/Bitfeld#Bit_auslesen
* @return change in encoder state (-1,0,1)
*/
int8_t DeviceEncoder::readEncoder() {
	static int8_t enc_states[] = { 0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0 };
	static uint8_t old_AB = 0;
	old_AB <<= 2;                 //remember previous state
	old_AB |= (ENC_PORT & 0x03);  //add current state
	return (enc_states[(old_AB & 0x0f)]);
}

/**
* Sets the Color and Brightness of the RGB-LED
* with PWM
*
* @param byte red   (0..255)
* @param byte green (0..255)
* @param byte blue  (0..255)
*/
void DeviceEncoder::setRgbLed(byte r, byte g, byte b) {
	/*
	// set 3 PWM PINs
	analogWrite(PIN_ENCODER_LED_R, r);
	analogWrite(PIN_ENCODER_LED_G, g);
	analogWrite(PIN_ENCODER_LED_B, b);
	*/
	mRgbLed->setRGB(r, g, b);
}

DeviceLED *DeviceEncoder::getRgbLed()
{
	return mRgbLed;
}


void DeviceEncoder::demoRotary() {
	static uint8_t counter = 0;      //this variable will be changed by encoder input
	int8_t value;
	value = readEncoder();
	if (value) {
		Serial.print("Counter value: ");
		Serial.println(counter, DEC);
		counter += value;
	}
}
