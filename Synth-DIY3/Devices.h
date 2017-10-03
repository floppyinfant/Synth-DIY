/**/

#ifndef WORKSTATION_H
#define WORKSTATION_H

#include "Workstation.h"

#include "DeviceShiftRegister.h"
#include "DeviceDisplay.h"
#include "DeviceTrellis.h"
#include "DeviceEncoder.h"
#include "DeviceNeopixels.h"
#include "DeviceMIDI.h"


class Devices {
public:
	Devices(void);
	~Devices(void);

	// ----------------------------------------------------------------------------

	void init();
	void update(void);
	
	// ----------------------------------------------------------------------------

	// EEPROM
	void loadSettings();
	void saveSettings();

	// UI
	void updateMenu(void);
	void updateViews(void);
	
	// Inputs
	void getSerialIn(void);
	// MIDI-Input --> handler
	void getButtonState(void);
	void getEncoderChange(void);
	void getTrellisButtonState(void);
	void getArduinoInputs();

	// Signals I/O
	void sendMIDI(int, int, int);
	void sendNoteOn(byte);
	void sendCVGate(byte);
	void sendCV(int);
	void sendGate(byte);
	void sendSynth(byte);
	
	// ----------------------------------------------------------------------------

	// Device Wrapper Classes
	// TODO: hide as private members (name: mShift...) or public (name: alpha4, trellis, ...) ?
	DeviceShiftRegister devShiftRegister;
	DeviceDisplay devDisplay;
	DeviceTrellis devTrellis;
	DeviceEncoder devEncoder;
	DeviceNeopixels devNeopixels;
	DeviceMIDI devMIDI;


private:
	void* mSerialConsole;
	
	// data
	int* mAnalogInputs; // Joystick x, y, (push); Ribbon
	int* mJoystick;
	int* mRibbon;

};


#endif // WORKSTATION_H
