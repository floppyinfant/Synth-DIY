/**/

#include "Devices.h"

Devices::Devices(void)
{
	
	//this->devTrellis = DeviceTrellis();
	devTrellis.trellisDemo();

	// ----------------------------------------------------------- old
	/*
	// @see header file: private members

	// MIDI; TODO: resolve Makros
	//this->midiSerial = midiSerial(PIN_MIDI_IN, PIN_MIDI_OUT);
	//MIDI_CREATE_INSTANCE(SoftwareSerial, midiSerial, MIDI);

	this->alpha4 = Adafruit_AlphaNum4();
	this->matrix0 = Adafruit_Trellis();
	this->trellis = Adafruit_TrellisSet(&matrix0);
	this->strip = Adafruit_NeoPixel(NUM_PIXELS, PIN_NEO, NEO_GRB + NEO_KHZ800);
	*/
}

Devices::~Devices(void)
{
}
