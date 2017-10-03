/** SHIFT-REGISTER: LED-ARRAY
* 74HC595 serial to parallel shift registers
* http://bildr.org/2011/02/74hc595/
* http://arduino.cc/en/Tutorial/ShiftOut
*/

#ifndef DEVICE_SHIFT_REGISTER_H
#define DEVICE_SHIFT_REGISTER_H

#include "Workstation.h"

#define number_of_74hc595s     3                         // change this: how many of the shift registers
#define NUM_REGISTER_PINS      number_of_74hc595s * 8    // do not touch

// PINs: Shift Register (Q0-Q7) and MUX1: buttons[]
// first shift register: set LEDs of button array on/off
#define BTN_01                 0
#define BTN_02                 1
#define BTN_03                 2
#define BTN_04                 3
#define BTN_05                 4
#define BTN_06                 5
// second shift-register: set LEDs of button array on/off
#define BTN_07                 6
#define BTN_08                 7
#define BTN_09                 8
#define BTN_10                 9
#define BTN_11                10
#define BTN_12                11
#define BTN_PLUS              12    // red
#define BTN_MINUS             13    // red
//#define                       14
#define GATE                  15    // shift out
//#define ENC_PUSH              14    // read by MUX 1
//#define JOY_PUSH              15    // read by MUX 1
// third shift-register: control MUX1 + MUX2
#define MUX1_S0               16    // set MUX1 (4 bytes) to read digital button state on PIN A1
#define MUX1_S1               17
#define MUX1_S2               18
#define MUX1_S3               19
#define MUX2_S0               20    // set MUX2 (4 Bits) to read analog potentiometer values on PIN A2
#define MUX2_S1               21
#define MUX2_S2               22
#define MUX2_S3               23

#define OFFSET_MUX1           16
#define OFFSET_MUX2           20

#define CANCEL                 0
#define ENTER                  1

// device MUX (x2) -> buttons[], potis[], PIN_ANALOG_IN_... --------------------
// button-array
#define NUM_POTIS              8
#define NUM_BUTTONS           16
#define DEBOUNCE_INTERVALL    10    // ms: 5+ is recommended by Adafruit

// MUX 2: index for potis[]
#define POTI_01                0
#define POTI_02                1
#define POTI_03                2
#define POTI_04                3
#define POTI_05                4
#define POTI_06                5
#define POTI_07                6
#define POTI_08                7
#define POTI_09                8
#define POTI_10                9
#define POTI_11               10
#define POTI_12               11
// Arpeggiator direct controlls
#define ARP_LATCH             12
#define ARP_RANGE             13
#define ARP_DIR_UP            14
#define ARP_DIR_DWN           15


class DeviceShiftRegister {
public:
	DeviceShiftRegister();
	~DeviceShiftRegister();

	// method chaining: 'return *this;'
	DeviceShiftRegister& clearRegisters(void);
	DeviceShiftRegister& writeRegisters(void);
	DeviceShiftRegister& setRegister(int, int);

	void update();
	void getButtonState();

	void demoShift(void);

private:
	/*
	// more objects for encapsulation:
	void* mMux;
	void* mButtons;
	void* mPotis;
	*/

	// data shiftRegister
	byte* mRegisters;

	// data buttonLEDs (OUT)
	byte* mButtonLEDs;

	// data buttons (IN)
	byte* mButtonsState;
	byte* mButtonsJustPressed;
	byte* mButtonsJustReleased;

	// data potis (IN)
	int* mPotisState;

};


#endif // !DEVICE_SHIFT_REGISTER_H
