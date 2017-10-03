#ifndef DEVICE_DAC_H
#define DEVICE_DAC_H

#include "Workstation.h"

#define ADDR_DAC            0x60    //1100010 or 1100011 (if select pin A0 is high)
#define DAC_RESOLUTION         8    // Set this value to 9 (512 values), 8 (256 values), 7 (128 values), 6 (64 values) or 5 (32 values) to adjust the resolution

class DeviceDAC {
public:
	DeviceDAC();
	~DeviceDAC();

	Adafruit_MCP4725 dac;

private:

};

#endif
