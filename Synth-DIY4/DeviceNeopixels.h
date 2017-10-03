/** Neopixels
* strip ws2812 with 32 RGB-pixels
*/

#ifndef DEVICE_NEOPIXELS_H
#define DEVICE_NEOPIXELS_H

#include "Workstation.h"

#define NUM_PIXELS            32


class DeviceNeopixels {
public:
	DeviceNeopixels();
	~DeviceNeopixels();

	uint32_t Wheel(byte);
	void colorWipe(uint32_t, uint8_t);
	void rainbow(uint8_t);

	//Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN_NEO, NEO_GRB + NEO_KHZ800);
	Adafruit_NeoPixel strip;

private:
	// data
	byte* mNeopixels; // RGB(A)

};


#endif
