/** MIDI
* MIDI is a serial protocol that operates at 31,250 bits per second.
*
* MIDI-IN: clock, note on/ off, program change, CC, PB
* MIDI-OUT:
*
* https://www.arduino.cc/en/Tutorial/Midi
* https://github.com/FortySevenEffects/arduino_midi_library
*/

#ifndef DEVICE_MIDI_H
#define DEVICE_MIDI_H

#include "Sequencer.h"


class DeviceMIDI
{
public:
	DeviceMIDI();
	~DeviceMIDI();

	// ----------------------------------------------------------------------------

	// Declaration
	//SoftwareSerial midiSerial(PIN_MIDI_IN, PIN_MIDI_OUT);
	//MIDI_CREATE_INSTANCE(SoftwareSerial, midiSerial, MIDI);

	static SoftwareSerial midiSerial;
	static midi::MidiInterface<SoftwareSerial> MIDI;
	
	// ----------------------------------------------------------------------------

	// member function
	void sendMIDI(int note, int vel, int ch);

	// Handler
	static void handleNoteOn(byte channel, byte pitch, byte velocity);
	static void handleNoteOff(byte channel, byte pitch, byte velocity);

	static void handleControlChange(byte channel, byte number, byte value);
	static void handlePitchBend(byte channel, int bend);
	static void handleProgramChange(byte channel, byte number);

	static void handleClock();

	static void handleStart();
	static void handleStop();
	static void handleContinue();

};

#endif // !DEVICE_MIDI_H
