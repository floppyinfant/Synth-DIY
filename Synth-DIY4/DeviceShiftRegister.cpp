#include "DeviceShiftRegister.h"

DeviceShiftRegister::DeviceShiftRegister():
	mRegisters(new byte[NUM_REGISTER_PINS]),
	mButtonsState(new byte[NUM_BUTTONS]),
	mButtonsJustPressed(new byte[NUM_BUTTONS]),
	mButtonsJustReleased(new byte[NUM_BUTTONS]),
	mPotisState(new int[NUM_BUTTONS])
{
	// Initialize Register
	clearRegisters();
	writeRegisters();
}

DeviceShiftRegister::~DeviceShiftRegister()
{
	delete[] mRegisters; mRegisters = 0;
	delete[] mButtonsState; mButtonsState = 0;
	delete[] mButtonsJustPressed; mButtonsJustPressed = 0;
	delete[] mButtonsJustReleased; mButtonsJustReleased = 0;
	delete[] mPotisState; mPotisState = 0;
}

/** set all register pins to LOW */
DeviceShiftRegister& DeviceShiftRegister::clearRegisters(void)
{
	for (int i = NUM_REGISTER_PINS - 1; i >= 0; i--) {
		mRegisters[i] = LOW;
	}
	return *this;
}

/** Set and display registers
* Only call AFTER all values are set how you would like (slow otherwise)
*/
DeviceShiftRegister& DeviceShiftRegister::writeRegisters(void)
{
	digitalWrite(PIN_SHIFT_RCLK, LOW);

	for (int i = NUM_REGISTER_PINS - 1; i >= 0; i--) {
		digitalWrite(PIN_SHIFT_SCLK, LOW);
		digitalWrite(PIN_SHIFT_SER, mRegisters[i]);
		digitalWrite(PIN_SHIFT_SCLK, HIGH);

	}
	digitalWrite(PIN_SHIFT_RCLK, HIGH);

	return *this;
}

/** set an individual pin HIGH or LOW */
DeviceShiftRegister& DeviceShiftRegister::setRegister(int index, int value)
{
	mRegisters[index] = value;

	return *this;
}

void DeviceShiftRegister::demoShift(void)
{
	setRegister(2, HIGH);
	setRegister(3, HIGH);
	setRegister(4, LOW);
	setRegister(5, HIGH);
	setRegister(7, HIGH);

	writeRegisters();  //MUST BE CALLED TO DISPLAY CHANGES
					   //Only call once after the values are set how you need.
}

/* MUX (16 Channel analog Multiplexer) ************************************** */

/** MUX: BUTTON-ARRAY
* Multiplexer
* used to read buttons (1 Octave Keyboard with Shift | Mode Keys)
* http://bildr.org/2011/02/cd74hc4067-arduino/
*/

/** Debouncing
* https://blog.adafruit.com/2009/10/20/example-code-for-multi-button-checker-with-debouncing/
* https://www.arduino.cc/en/Tutorial/Debounce
* http://playground.arduino.cc/Code/Bounce
* https://github.com/thomasfredericks/Bounce2
*/

// TODO: Hardware Pullup-Resistor for Buttons

void DeviceShiftRegister::getButtonState() {

	static int muxChannel[16][4] = {
		{ 0,0,0,0 }, //channel 0
		{ 1,0,0,0 }, //channel 1
		{ 0,1,0,0 }, //channel 2
		{ 1,1,0,0 }, //channel 3
		{ 0,0,1,0 }, //channel 4
		{ 1,0,1,0 }, //channel 5
		{ 0,1,1,0 }, //channel 6
		{ 1,1,1,0 }, //channel 7
		{ 0,0,0,1 }, //channel 8
		{ 1,0,0,1 }, //channel 9
		{ 0,1,0,1 }, //channel 10
		{ 1,1,0,1 }, //channel 11
		{ 0,0,1,1 }, //channel 12
		{ 1,0,1,1 }, //channel 13
		{ 0,1,1,1 }, //channel 14
		{ 1,1,1,1 }  //channel 15
	};

	// Debounce
	static byte previousstate[NUM_BUTTONS];
	static byte currentstate[NUM_BUTTONS];
	static unsigned long lasttime;

	if ((millis() - lasttime) < DEBOUNCE_INTERVALL) {
		// not enough time has passed
		return;
	}
	// ok, we have waited DEBOUNCE_INTERVALL
	lasttime = millis(); // reset timer

	// iterate over the 16 channels of the mux
	for (int channel = 0; channel < NUM_BUTTONS; channel++) {
		// set shift-register
		for (int i = 0; i < 4; i++) {
			mRegisters[OFFSET_MUX1 + i] = muxChannel[channel][i];
			mRegisters[OFFSET_MUX2 + i] = muxChannel[channel][i];
		}
		writeRegisters();

		// read state
		if (channel < NUM_POTIS) {
			mPotisState[channel] = analogRead(PIN_MUX2_SIG);
		}
		int value = analogRead(PIN_MUX1_SIG); // 10-bit values: 0..1023
											  // convert from analog 10-bit values to digital values: 0|1
		if (value > 512) { // treshold for noise
			currentstate[channel] = HIGH;
		}
		else {
			currentstate[channel] = LOW;
		}

		// Debounce
		if (currentstate[channel] == previousstate[channel]) {
			if ((mButtonsState[channel] == LOW) && (currentstate[channel] == LOW)) {
				// just pressed
				mButtonsJustPressed[channel] = 1;
			}
			else if ((mButtonsState[channel] == HIGH) && (currentstate[channel] == HIGH)) {
				// just released
				mButtonsJustReleased[channel] = 1;
			}
			mButtonsState[channel] = !currentstate[channel];  // remember, digital HIGH means NOT pressed
		} // end if
		previousstate[channel] = currentstate[channel];
	} // end for channel
} // end getButtonStates()
