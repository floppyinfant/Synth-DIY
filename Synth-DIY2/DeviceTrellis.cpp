#include "DeviceTrellis.h"

DeviceTrellis::DeviceTrellis()
{
	this->trellis.begin(ADDR_TRELLIS);
}

DeviceTrellis::~DeviceTrellis()
{
}

void DeviceTrellis::trellisReadButtons() {
	delay(30); // 30ms delay is required, dont remove me!

	if (MODE == MOMENTARY) {
		// If a button was just pressed or released...
		if (trellis.readSwitches()) {
			// go through every button
			for (uint8_t i = 0; i < NUM_TRELLIS_BUTTONS; i++) {
				// if it was pressed, turn it on
				if (trellis.justPressed(i)) {
					Serial.print("v"); Serial.println(i);
					trellis.setLED(i);
				}
				// if it was released, turn it off
				if (trellis.justReleased(i)) {
					Serial.print("^"); Serial.println(i);
					trellis.clrLED(i);
				}
			}
			// tell the trellis to set the LEDs we requested
			trellis.writeDisplay();
		}
	}

	if (MODE == LATCHING) {
		// If a button was just pressed or released...
		if (trellis.readSwitches()) {
			// go through every button
			for (uint8_t i = 0; i < NUM_TRELLIS_BUTTONS; i++) {
				// if it was pressed...
				if (trellis.justPressed(i)) {
					Serial.print("v"); Serial.println(i);
					// Alternate the LED
					if (trellis.isLED(i))
						trellis.clrLED(i);
					else
						trellis.setLED(i);
				}
			}
			// tell the trellis to set the LEDs we requested
			trellis.writeDisplay();
		}
	}
}

/**
* Sets the appropriate LEDs
*
* @param byte[] pattern - must be 16 in length (0|1)
*/
void DeviceTrellis::trellisWrite(byte pattern[]) {
	for (byte i = 0; i < NUM_TRELLIS_BUTTONS; i++) {
		if (pattern[i] == 0) {
			trellis.clrLED(i);
		}
		else {
			trellis.setLED(i);
		}
	}
	trellis.writeDisplay();
}

void DeviceTrellis::trellisDemo() {
	// light up all the LEDs in order
	for (uint8_t i = 0; i < NUM_TRELLIS_BUTTONS; i++) {
		trellis.setLED(i);
		trellis.writeDisplay();
		delay(50);
	}
	// then turn them off
	for (uint8_t i = 0; i < NUM_TRELLIS_BUTTONS; i++) {
		trellis.clrLED(i);
		trellis.writeDisplay();
		delay(50);
	}
}
