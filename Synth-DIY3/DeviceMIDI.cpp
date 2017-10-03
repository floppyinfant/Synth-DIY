#include "DeviceMIDI.h"

// Declaration, Definition, Initialization: static variables not in class definition or constructor!
SoftwareSerial DeviceMIDI::midiSerial(PIN_MIDI_IN, PIN_MIDI_OUT);
midi::MidiInterface<SoftwareSerial> DeviceMIDI::MIDI(midiSerial);

DeviceMIDI::DeviceMIDI()/* :
	midiSerial(PIN_MIDI_IN, PIN_MIDI_OUT),
	MIDI(midiSerial)*/
{
    // Declaration, Definition, Initialization
	//SoftwareSerial midiSerial = SoftwareSerial(PIN_MIDI_IN, PIN_MIDI_OUT);
	//MIDI_CREATE_INSTANCE(SoftwareSerial, midiSerial, MIDI);

	// ----------------------------------------------------------------------------

	// Register event listeners | event handler | callbacks
	MIDI.setHandleNoteOn(handleNoteOn);
	MIDI.setHandleNoteOff(handleNoteOff);
	
	MIDI.setHandleControlChange(handleControlChange);
	MIDI.setHandlePitchBend(handlePitchBend);
	MIDI.setHandleProgramChange(handleProgramChange);

	MIDI.setHandleClock(handleClock);
	MIDI.setHandleStart(handleStart);
	MIDI.setHandleStop(handleStop);
	MIDI.setHandleContinue(handleContinue);
	
	// unregister a callback-handler
	// MIDI.disconnectCallbackFromType(handler); 

	// ----------------------------------------------------------------------------

	MIDI.begin(MIDI_CHANNEL_OMNI); // MIDI_CHANNEL_OMNI
}


DeviceMIDI::~DeviceMIDI()
{
}


void DeviceMIDI::sendMIDI(int note, int vel, int ch) {
	MIDI.sendNoteOn(note, vel, ch);  // Send a Note (pitch 42, velo 127 on channel 1)
									 //delay(1000);		           // Wait for a second
									 //MIDI.sendNoteOff(note, 0, ch); // Stop the note
}


/** CALLBACKS *************************************************************** */

/** This function will be automatically called when a NoteOn is received.
* It must be a void-returning function with the correct parameters,
* see documentation here:
* http://arduinomidilib.fortyseveneffects.com/a00022.html
*/
void DeviceMIDI::handleNoteOn(byte channel, byte pitch, byte velocity) {
	// Do whatever you want when a note is pressed.

	// Try to keep your callbacks short (no delays ect)
	// otherwise it would slow down the loop() and have a bad impact
	// on real-time performance.

	//unsigned int freq = mtof(pitch);
	// scale and use by DAC as CV

	MIDI.turnThruOff();
}

void DeviceMIDI::handleNoteOff(byte channel, byte pitch, byte velocity) {
	// Do something when the note is released.
	// Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.

	// @see libraries/MIDI/examples/MIDI_SimpleSynth
}

void DeviceMIDI::handleControlChange(byte channel, byte number, byte value) {
	switch (number) {
	case 1:
		// do something with modulation wheel
		break;
	case 100:
		// TODO: Example CC 100 (volume)
		int vol = map(value, 0, 127, 0., 1.);
		break;
	}
}

void DeviceMIDI::handlePitchBend(byte channel, int bend) {

}

void DeviceMIDI::handleProgramChange(byte channel, byte number) {

}

void DeviceMIDI::handleClock() {
	// 24ppq clock pulses

	/*
	// @see Groovesizer HandleMidi.ino
	lastClock = millis();
	seqRunning = false;
	pulse++; // 6 for a 16th
	autoCounter = (autoCounter < 95) ? autoCounter + 1 : 0;
	static boolean long16th = true; // is this the first/longer 16th of the swing pair?
	char swingPulse = swing / 86; // to give us some swing when synced to midi clock (sadly only 2 levels: full swing and half swing)
	// takes a value between 0 and 255 and returns 0, 1 or 2
	swingPulse = (!long16th) ? (0 - swingPulse) : swingPulse;
	if (pulse >= (6 + swingPulse))
	{
	currentTime = millis();
	pulse = 0;
	seqMidiStep = true;
	//seqTrueStep = (seqTrueStep + 1)%32;
	if (seqNextStep == -1 && seqCurrentStep == 0) { // special case for reversed pattern
	seqCurrentStep = seqLength - 1;
	} else {
	seqCurrentStep = (seqCurrentStep + seqNextStep)%seqLength; // advance the step counter
	}
	long16th = !long16th;

	sixteenthStartTime = millis();
	*/
}

void DeviceMIDI::handleStart() {
	// reset all counter
	// start sequencer
}

void DeviceMIDI::handleStop() {
	// stop all MIDI-Notes playing
	for (byte i = 0; i < 128; i++) // turn off all midi notes
		DeviceMIDI::MIDI.sendNoteOff(i, 0, sequencer.midi.channel);
	// stop sequencer
}

void DeviceMIDI::handleContinue() {

}


/* *****************************************************************************
// more callback-handler signatures: register callback with 'MIDI.setHandle...(handle...);'
void HandleNoteOff (byte channel, byte note, byte velocity)
void HandleNoteOn (byte channel, byte note, byte velocity)
void HandleAfterTouchPoly (byte channel, byte note, byte pressure)
void HandleControlChange (byte channel, byte number, byte value)
void HandleProgramChange (byte channel, byte number)
void HandleAfterTouchChannel (byte channel, byte pressure)
void HandlePitchBend (byte channel, int bend)
void HandleSystemExclusive (byte *array, byte size)
void HandleTimeCodeQuarterFrame (byte data)
void HandleSongPosition (unsigned int beats)
void HandleSongSelect (byte songnumber)
void HandleTuneRequest (void)
void HandleClock (void)
void HandleStart (void)
void HandleContinue (void)
void HandleStop (void)
void HandleActiveSensing (void)
void HandleSystemReset (void)
*/
