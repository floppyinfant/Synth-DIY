#include "Tests.h"


// -----------------------------------------------------------------------------
/* TESTS */
// -----------------------------------------------------------------------------

void test() {

	Serial.println(F("TEST MODE ----------------------"));

	/*
#ifdef DEVICE_SERIAL_IN
	getSerialIn();
#else
	//testUnit(DEVICE_DISPLAY);
	//testAll();
	blink(3000);
#endif

	// TESTS:
	// blink LED
	if (currentMillis - previousMillis < 200) {
		blink(100);
	}
	else if (currentMillis - previousMillis > 2000 && currentMillis - previousMillis < 4000) {
		blink(50);
	}
	else if (currentMillis - previousMillis > 4000) {
		blink(200);
	}

	// datastructures
	// EEPROM
	// every function

	// timing, profile loop
	// sequencer

	// DEVICES:
	// MIDI

	// Device alpha4

	// Device Trellis

	// Device Rotary Encoder
	demoRotary();
	
	Serial.println(F("RBG red"));
	setRGBLED(255, 0, 0);
	delay(1000);
	Serial.println(F("RBG green"));
	setRGBLED(0, 255, 0);
	delay(1000);
	Serial.println(F("RBG blue"));
	setRGBLED(0, 0, 255);
	delay(1000);

	rainbow(200);

	// Device Shift Register (-->LEDs), MUX (<-- Buttons, Potis)

	// TODO:
	// implement Arpeggiator
	// implement Scales
	*/
}


void testAll() {
	for (int i = 0; i < 13; i++) {
		testUnit(i);
	}
}


void testUnit(int UNIT) {
	switch (UNIT) {
#ifdef DEVICE_ARDUIUNO_PINS
	case DEVICE_ARDUIUNO_PINS:
		break;
#endif
#ifdef DEVICE_EEPROM
	case DEVICE_EEPROM:
		break;
#endif
#ifdef DEVICE_PROGMEM
	case DEVICE_PROGMEM:
		break;
#endif
#ifdef DEVICE_SERIAL_IN
	case DEVICE_SERIAL_IN:
		break;
#endif
#ifdef DEVICE_MIDI
	case DEVICE_MIDI:
		break;
#endif
#ifdef DEVICE_DAC
	case DEVICE_DAC:
		break;
#endif
#ifdef DEVICE_SYNTH_AUDUINO
	case DEVICE_SYNTH_AUDUINO:
		break;
#endif
#ifdef DEVICE_SHIFT_REGISTERS
	case DEVICE_SHIFT_REGISTERS:
		break;
#endif
#ifdef DEVICE_MUX
	case DEVICE_MUX:
		break;
#endif
#ifdef DEVICE_ENCODER
	case DEVICE_ENCODER:
		demoRotary();
		break;
#endif
#ifdef DEVICE_NEOPIXELS
	case DEVICE_NEOPIXELS:
		rainbow(200);
		break;
#endif
#ifdef DEVICE_DISPLAY
	case DEVICE_DISPLAY:
		break;
#endif
#ifdef DEVICE_TRELLIS
	case DEVICE_TRELLIS:
		break;
#endif
	default:
		Serial.println(F("DEVICE not defined"));
		break;
	}
}

