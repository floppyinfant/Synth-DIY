#include "DeviceDAC.h"

DeviceDAC::DeviceDAC() :
	dac()
{
	// Deklaration, Definition, Initialization
	//Adafruit_MCP4725 dac;

	// For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
	// For Adafruit MCP4725A0 the address is 0x60 or 0x61
	// For Adafruit MCP4725A2 the address is 0x64 or 0x65
	dac.begin(ADDR_DAC);

}

DeviceDAC::~DeviceDAC()
{
}


/**
* CAVE: very time consumung: 350 - 400ms
* (Better use LUT)
* TODO dynamische Programmierung or LUT
*
* @see Pure Data (PD)
* @see Mozzi
* https://github.com/vishnubob/armi/blob/master/firmware/mozzi/utils.cpp
*/
float DeviceDAC::midiToCv(byte midival) {
	return 8.1757989156 * pow(2.0, (float)midival / 12.0);
}
