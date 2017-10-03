#pragma once

#include "Workstation.h"

class DeviceNeopixels {
public:
	DeviceNeopixels();
	~DeviceNeopixels();

	uint32_t Wheel(byte);
	void colorWipe(uint32_t, uint8_t);
	void rainbow(uint8_t);

private:

};