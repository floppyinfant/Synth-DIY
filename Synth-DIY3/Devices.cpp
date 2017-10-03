/**/

#include "Devices.h"

Devices::Devices(void) :
	devShiftRegister(),
	devDisplay(),
	devTrellis(),
	devEncoder(),
	devNeopixels(),
	devMIDI()
{
	
}

Devices::~Devices(void)
{
}

// ----------------------------------------------------------------------------

void Devices::loadSettings() {
	int addr = 0;
	/*
	if (EEPROM.read(0) != 0xff) {

	for (int i=0; i < NUM_STEPS; i++) {
	sequence[i] = EEPROM.read(addr);
	addr++;
	}
	for (int i=0; i < NUM_STEPS; i++) {
	sequence_slides[i] = EEPROM.read(addr);
	addr++;
	}
	for (int i=0; i < NUM_STEPS; i++) {
	sequence_velocities[i] = EEPROM.read(addr);
	addr++;
	}
	for (int i=0; i < NUM_STEPS; i++) {
	sequence_ranges[i] = EEPROM.read(addr);
	addr++;
	}
	for (int i=0; i < NUM_STEPS; i++) {
	sequence_directions[i] = EEPROM.read(addr);
	addr++;
	}
	sequence_length = EEPROM.read(addr); addr++;
	for (int i=0; i < NUM_STEPS; i++) {
	transpositions[i] = EEPROM.read(addr);
	addr++;
	}
	transpositions_length = EEPROM.read(addr); addr++;
	var_transpose = EEPROM.read(addr); addr++;
	var_inversion = EEPROM.read(addr); addr++;
	var_mirror_at = EEPROM.read(addr); addr++;
	var_reversing = EEPROM.read(addr); addr++;
	arp_length = EEPROM.read(addr); addr++;
	arp_scale = EEPROM.read(addr); addr++;
	arp_hold = EEPROM.read(addr); addr++;
	arp_octaves = EEPROM.read(addr); addr++;
	arp_direction = EEPROM.read(addr); addr++;
	arp_rate = EEPROM.read(addr); addr++;
	arp_reset = EEPROM.read(addr); addr++;
	midiChannel = EEPROM.read(addr); addr++;
	midiOutChannel = EEPROM.read(addr); addr++;
	midiSync = EEPROM.read(addr); addr++;
	tempo = EEPROM.read(addr); addr++;
	quantization = EEPROM.read(addr); addr++;
	} // end if
	*/
}

void Devices::saveSettings() {
	/*
	//EEPROM.write(addr, byte);
	int addr = 0;

	for (int i=0; i < NUM_STEPS; i++) {
	EEPROM.write(addr, sequence[i]);
	addr++;
	}
	for (int i=0; i < NUM_STEPS; i++) {
	EEPROM.write(addr, sequence_slides[i]);
	addr++;
	}
	for (int i=0; i < NUM_STEPS; i++) {
	EEPROM.write(addr, sequence_velocities[i]);
	addr++;
	}
	for (int i=0; i < NUM_STEPS; i++) {
	EEPROM.write(addr, sequence_ranges[i]);
	addr++;
	}
	for (int i=0; i < NUM_STEPS; i++) {
	EEPROM.write(addr, sequence_directions[i]);
	addr++;
	}
	EEPROM.write(addr, sequence_length); addr++;
	for (int i=0; i < NUM_STEPS; i++) {
	EEPROM.write(addr, transpositions[i]);
	addr++;
	}
	EEPROM.write(addr, transpositions_length); addr++;
	EEPROM.write(addr, var_transpose); addr++;
	EEPROM.write(addr, var_inversion); addr++;
	EEPROM.write(addr, var_mirror_at); addr++;
	EEPROM.write(addr, var_reversing); addr++;
	EEPROM.write(addr, arp_length); addr++;
	EEPROM.write(addr, arp_scale); addr++;
	EEPROM.write(addr, arp_hold); addr++;
	EEPROM.write(addr, arp_octaves); addr++;
	EEPROM.write(addr, arp_direction); addr++;
	EEPROM.write(addr, arp_rate); addr++;
	EEPROM.write(addr, arp_reset); addr++;
	EEPROM.write(addr, midiChannel); addr++;
	EEPROM.write(addr, midiOutChannel); addr++;
	EEPROM.write(addr, midiSync); addr++;
	EEPROM.write(addr, tempo); addr++;
	EEPROM.write(addr, quantization); addr++;
	*/
}

// ----------------------------------------------------------------------------

void Devices::updateMenu() {
	// switch (MENU)
	// setParam..., updateSettings(), saveSettings()
}

void Devices::updateViews() {
	// alphanumDisplay
	// buttonArray
	// RGBLED
	// trellisLEDArray
	// neopixels
}

// ----------------------------------------------------------------------------

/** Serial Communication */

// Serial Communication with PC via USB-Cable
// https://www.arduino.cc/en/Tutorial/SwitchCase2
// http://playground.arduino.cc/Main/InterfacingWithHardware#Communication

// Pure Data: PDuino
// http://at.or.at/hans/pd/objects.html

// Raspberry Pi: Pygame, cwiid, DAW | VST
// http://www.pygame.org/docs/ref/midi.html
// https://github.com/abstrakraft/cwiid

// Serial-MIDI-Converter
// http://www.spikenzielabs.com/SpikenzieLabs/Serial_MIDI.html


void Devices::getSerialIn() {
	Serial.println(F("Please insert command <char val>: "));
	Serial.println(F("  commands: b ms   - blink LED with intervall ms"));
	Serial.println(F("            a txt  - write txt to alphanum display"));
	Serial.print(F("$ "));

	//while (! Serial.available()) return;
	if (Serial.available() > 0) {
		char c = Serial.read();
		//if (! isprint(c)) return; // only printable!

		if (Serial.available() > 0) {
			// space
			if (Serial.read() != ' ') {
				Serial.print(F("expected space after command "));
				Serial.println(c);
				return;
			}
		}

		// buffer
		char s[100];
		int i = 0;
		while (Serial.available() > 0) {
			*(s + i) = Serial.read();
			if (++i >= 100) {
				// buffer full
				break;
			};

		}

		//int command = Serial.parseInt();
		Serial.print(F("received command: "));
		Serial.print(c);

		Serial.print(F(" with value "));
		Serial.println(s);

		if (c == 't') {             // set Tempo
									// stoi(s);
		}
		else if (c == 'b') {      // blink LED PIN13

		} // more commands
	}
}

void getEncoderChange() {
}

void getTrellisButtonState() {
}

void getArduinoInputs() {
}

