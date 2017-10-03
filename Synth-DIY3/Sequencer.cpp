#include "Sequencer.h"

using namespace std;

// Constructor
Sequencer::Sequencer(void)
{
}

// Destructor
Sequencer::~Sequencer(void)
{
}

void Sequencer::update()
{
}

// ----------------------------------------------------------------------------

// TODO make C functions members of class sequencer
/*
void updateSequencer() {
	// getClock()
	// @see handleClock

	if (MODE == MODE_PAUSE || MODE == MODE_STOP) {
		return;
	}

	if (millis() >= sequencer.timecode + sequencer.timestep) {
		sequencer.timecode += sequencer.timestep; //sequencer.timecode = millis();

												  // update sequence position
												  // variation reverse //////////////////////////////////////////////////
		if (sequencer.variation.reverse.active) {
			// reverse
			sequencer.position--;
		}
		else {
			sequencer.position++;
		}

		if (sequencer.position >= sequencer.song.pattern[0].track[0].sequence.length) {
			// reset position
			sequencer.position = 0;

			// update variations position
			sequencer.variation.transposition.position++;
			if (sequencer.variation.transposition.position >= sequencer.variation.transposition.length) {
				sequencer.variation.transposition.position = 0;
			}
			sequencer.variation.mirror.position++;
			if (sequencer.variation.mirror.position >= sequencer.variation.mirror.length) {
				sequencer.variation.mirror.position = 0;
			}

		}
		else if (sequencer.position < 0) {
			// reverse variation mode
			sequencer.position = sequencer.song.pattern[0].track[0].sequence.length - 1;
		}

		// get note
		byte note;

		// variation chord inversion (1./2. Umkehrung) /////////////////////////
		byte i;
		if (sequencer.variation.inversion.type != 0) {
			if (sequencer.arpeggiator.length == 0) {
				i = 3;
			}
			else {
				i = sequencer.arpeggiator.length;
			}
			switch (sequencer.variation.inversion.type) {
			case 1:
				// Sextakkord
				if (sequencer.position % i == 0) {
					// position is 0, 3, 6, ... --> return value at position 1
					note = sequencer.song.pattern[0].track[0].sequence.step[sequencer.position + 1].note;
				}
				else if ((sequencer.position - 1) % i == 0) {
					// position 1, 4, ... --> return value at position 2
					note = sequencer.song.pattern[0].track[0].sequence.step[sequencer.position + 1].note;
				}
				else if ((sequencer.position - 2) % i == 0) {
					// position 2, 5, ... --> return value at position 0 - 12 half-steps (1 octave down)
					note = sequencer.song.pattern[0].track[0].sequence.step[sequencer.position - 2].note - 12;
				}
				else {
					note = sequencer.song.pattern[0].track[0].sequence.step[sequencer.position].note;
				}
			case 2:
				// Quartsextakkord
				if (sequencer.position % i == 0) {
					// return value at position 2
					note = sequencer.song.pattern[0].track[0].sequence.step[sequencer.position + 2].note;
				}
				else if ((sequencer.position - 1) % i == 0) {
					// return value at position 0 -12 half-steps (1 octave down)
					note = sequencer.song.pattern[0].track[0].sequence.step[sequencer.position - 1].note - 12;
				}
				else if ((sequencer.position - 2) % i == 0) {
					// return value at position 1 - 12 half-steps (1 octave down)
					note = sequencer.song.pattern[0].track[0].sequence.step[sequencer.position - 1].note - 12;
				}
				else {
					note = sequencer.song.pattern[0].track[0].sequence.step[sequencer.position].note;
				}
			}
		}
		else {
			note = sequencer.song.pattern[0].track[0].sequence.step[sequencer.position].note;
		}

		// variation mirror ////////////////////////////////////////////////////
		byte mirror = sequencer.variation.mirror.sequence[sequencer.variation.mirror.position];
		note = (mirror - note) + mirror; // add the difference to mirror

										 // variation transposition /////////////////////////////////////////////
		byte transposition = sequencer.variation.transposition.sequence[sequencer.variation.transposition.position];

		// combine note of sequence with transposition and varition matrices
		note = note + transposition;


		// send note
		//sendMIDI(note, vel, ch);
		sendCVGate(note);
		sendSynth(note);

	}
	else {
		//return;
	}
}

void startSequencer() {

	switch (MODE) {
	case MODE_PLAY:
		playSequence();
	case MODE_REC:
		recordSequence();
	}
}

void stopSequencer() {
}

void pauseSequencer() {
}

void playSequence() {
	// @see updateSequencer()
}

void recordSequence() {
}

void sendNoteOn(byte note) {
}

void sendCVGate(byte note) {
}

// send commands to Auduino
void sendSynth(byte note) {
}

*/

// ----------------------------------------------------------------------------

int Sequencer::getTime()
{
	return 0;
}

int Sequencer::getStep()
{
	return 0;
}

/**
* (1 min * 60 s * 1000 ms) / (bpm beats * 4 steps) => [ms/step]
*
* @return the time for 1 steps (1/16 note) in ms.
*
*/
float Sequencer::getTimePerStepFromBpm(int bpm)
{
	return 15000 / bpm;
}

/**
* CAVE: very time consumung: 350 - 400ms
* (Better use LUT)
*
* @see Pure Data (PD)
* @see Mozzi
* https://github.com/vishnubob/armi/blob/master/firmware/mozzi/utils.cpp
*/
float Sequencer::mtof(byte midival)
{
	return 8.1757989156 * pow(2.0, (float)midival / 12.0);
}
