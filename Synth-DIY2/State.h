#pragma once

#include "Workstation.h"

class State {
public:

	struct in {

		struct buttons {
			byte buttonstate[NUM_BUTTONS];
			byte justpressed[NUM_BUTTONS];
			byte justreleased[NUM_BUTTONS];
		} buttons;

		int potis[NUM_POTIS];    // analog data is 10 bits

		byte trellis[16];

		byte encoder;

		struct arduino {
			struct joy {
				byte x;
				byte y;
				byte pushed;
			} joy;

			byte ribbon;
			// more?

		} arduino;

	} in;

	struct out {
		byte registers[NUM_REGISTER_PINS];    // 3 shift registers

		byte buttonLEDs[16];
		byte RGBLED[3];
		char alphanumDisplay[NUM_DIGITS];
		byte trellis[16];
#ifdef DEVICE_NEOPIXELS
		byte neopixels[32];
#endif // DEVICE_NEOPIXELS

	} out;
private:

};

