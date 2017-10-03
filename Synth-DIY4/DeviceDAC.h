/**
Digital Analog Converter (DAC)
12-bit resolution: 4096 values scaled on Input voltage (VCC), e.g. 5V (Arduino)
This board/chip uses I2C 7-bit address, you can use 0x62 or 0x63


WIRING: Connect ...
- VDD (power) to a 3-5V power supply, and GND to ground.

- SDA to I2C Data (A4 on the UNO/ Nano)
- SCL  to I2C Clock (A5 on the UNO/ Nano)

- A0 allow you to change the I2C address. By default (nothing attached to A0) 
  the address is hex 0x62. 
  If A0 is connected to VDD the address is 0x63. 
  This lets you have two DAC boards connected to the same SDA/SCL I2C bus pins.

- VOUT is the voltage out from the DAC! 
  The voltage will range from 0V (when the DAC value is 0) to VDD (when the DAC 'value' is the max 12-bit number: 0xFFF)


REFERENCES:

Adafruit
https://www.adafruit.com/product/935
Tutorials
https://learn.adafruit.com/mcp4725-12-bit-dac-tutorial
https://learn.adafruit.com/mcp4725-12-bit-dac-with-raspberry-pi

Sparkfun MIDI-Shield: MIDI to CV
https://github.com/sparkfun/MIDI_Shield/tree/V_1.5/Firmware/MIDI-CV
https://learn.sparkfun.com/tutorials/midi-shield-hookup-guide/example-2-midi-to-control-voltage

Adafruit FifteenStep Sequencer Library
https://github.com/adafruit/FifteenStep
Projects based on "Adafruit FifteenStep Sequencer Library"
https://gist.github.com/toddtreece/5449fdb479ddadd0ed5b

*/

#ifndef DEVICE_DAC_H
#define DEVICE_DAC_H

#include "Workstation.h"

#define ADDR_DAC            0x62    // 1100010 (0x62) or 1100011 (0x63) if select pin A0 is high
#define DAC_RESOLUTION         8    // Set this value to 9 (512 values), 8 (256 values), 7 (128 values), 6 (64 values) or 5 (32 values) to adjust the resolution

// -----------------------------------------------------------------------------
// Code from Sparkfun:
// https://github.com/sparkfun/MIDI_Shield/tree/V_1.5/Firmware/MIDI-CV
//
// Constants to describe the MIDI input:
// NUM_KEYS is the number of keys we're interpreting
// Vcc 5V is enought for 4 Octaves (@1V/Octave) + Pitch Bend
static const int8_t NUM_KEYS = 49;
// BASE_KEY is the offset of the lowest key number
static const int8_t BASE_KEY = 36;
// The tuning constant - representing the DAC counts per semitone
// Arrived at using: (((2^<DAC resolution>)/5)/12) * 100
//
// 2^12 (bit) = 4096 total DAC counts.
// 4096/5 = 819.2 DAC counts per volt on a 5V supply
// 819.2/12 = dac counts per semitone = 68.26
// times 100 for some extra calculation precision = 6826
static const uint32_t DAC_CAL = 6826;
// -----------------------------------------------------------------------------

class DeviceDAC {
public:
	DeviceDAC();
	~DeviceDAC();

	void sendNote(int);
	float midiToCv(byte);

private:
	Adafruit_MCP4725 dac;

};

#endif
