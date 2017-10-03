#pragma once

#include "Workstation.h"

class DeviceEncoder {
public:
	DeviceEncoder();
	~DeviceEncoder();

	int8_t readEncoder(void);
	void setRGBLED(byte, byte, byte);
	void demoRotary(void);

private:

};