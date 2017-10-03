/** ***************************************************************************
* Synthesizer Workstation
* Mars 1 | MADS (Musical Analog Digital Synthesizer | Sequencer)
* by Kreative Energie
* Dedicated to Mads Marian Ernst
*
* Description: Hardware modules coupled to make music
* Keywords: Physical Computing, Arduino Platform, Atmel AVR ATmega328P,
*           Arpeggiator, Step-Sequencer, Music Workstation,
*           Analog Synthesizer, CV/Gate, MIDI
*
*
* @author Totman (TM)
* @date 2016-03-02 (v0.5), 2014-11-20 (v0.1)
* @version 0.5
* @license Public Domain | CC | (L)GPL | Apache | BSD | MIT |Proprietary
* https://tldrlegal.com/
* Open Source, Open Hardware
*
* Copyright by Thorsten Mauthe
* floppyinfant.com - all right reserved
*
*****************************************************************************/

/* *****************************************************************************
* Header file: Interface, Declarations
* ************************************************************************** */

#pragma once

#ifndef SYNTH_WORKSTATION_H
#define SYNTH_WORKSTATION_H

/* *****************************************************************************
* Constants
* ************************************************************************** */

// #define ONE 1

// C++: better use 'const' (==> readable compiler messages on error)

/* *****************************************************************************
* Makros
* ************************************************************************** */

// #define ADD(a,b) ((a)+(b))

/* *****************************************************************************
* Type Declarations
* ************************************************************************** */

// typedef struct tag {} Type;

/* *****************************************************************************
* Global Variables
* ************************************************************************** */

// extern, static, const, volatile

/* *****************************************************************************
* Prototypes
* ************************************************************************** */

// identifier:
// setup(), init...()
// loop(), run(), go()
// get...(), read...()
// set...(), write...()
// update...()
// test...()
// demo...()


/* *****************************************************************************
* Cpp file: Implementation, Definitions
* ************************************************************************** */


// -----------------------------------------------------------------------------
/* ARDUINO */
// -----------------------------------------------------------------------------

#include "Arduino.h"

// Arduino Libraries

// Timer
// Programming model:
// state machine (non-blocking code without delay() but millis(), timers or interutpts)
// @see Adafruit https://learn.adafruit.com/multi-tasking-the-arduino-part-1
#include <SimpleTimer.h>            // 

// Debouncing
#include <Bounce2.h>                // Debouncing for button presses


// AVR-libc: Atmel AVR ATmega328P
// http://www.nongnu.org/avr-libc/user-manual/index.html
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>           // included so we can use PROGMEM


// Standard C Library
//#include <stddef.h>
#include <stdlib.h>                 // string to numbers: atoi, atol, atof, sprintf; malloc, ...
//#include <stdio.h>                // I/O: fopen, fclose, fseek, fflush, fgetc, fputc, fgets, fputs, fread, fwrite, scanf, printf (f=file pointer, c=char, s=string, f=formatstring, v=vector)
//#include <string.h>               // strcat, strcmp, strcpy, strlen, etc.
//#include <ctype.h>                // char-type testen | umwandeln: isalpha, isdigit, ..., toupper, tolower
//#include <math.h>


// Standard C++ Library
// STL (Standard Template Library)

// -----------------------------------------------------------------------------

void setup() {
	pinMode(PIN_AUDUINO_PWM, OUTPUT);
	//pinMode(PIN_AUDUINO_LED, OUTPUT); // is PIN 13, which is also used for MIDI-OUT

	pinMode(PIN_ENCODER_A, INPUT);
	digitalWrite(PIN_ENCODER_A, HIGH);
	pinMode(PIN_ENCODER_B, INPUT);
	digitalWrite(PIN_ENCODER_B, HIGH);
	pinMode(PIN_ENCODER_PUSH, INPUT);

	pinMode(PIN_ENCODER_LED_R, OUTPUT);
	pinMode(PIN_ENCODER_LED_G, OUTPUT);
	pinMode(PIN_ENCODER_LED_B, OUTPUT);

	pinMode(PIN_SHIFT_SER, OUTPUT);
	pinMode(PIN_SHIFT_SCLK, OUTPUT);
	pinMode(PIN_SHIFT_RCLK, INPUT);
	// MUX S0-S3 connected to shift-register (16-19 + 20-23)
	pinMode(PIN_MUX1_SIG, INPUT);
	pinMode(PIN_MUX2_SIG, INPUT);

	pinMode(PIN_MIDI_IN, INPUT);
	pinMode(PIN_MIDI_OUT, OUTPUT);
	pinMode(PIN_GATE, OUTPUT);

	pinMode(PIN_JOY_X, INPUT);
	pinMode(PIN_JOY_Y, INPUT);

}


// -----------------------------------------------------------------------------
/* DEVICES */
// -----------------------------------------------------------------------------

#ifdef DEVICE_EEPROM
#include <EEPROM.h>                 //for reading and writing patterns to EEPROM
#endif // DEVICE_EEPROM

// -----------------------------------------------------------------------------

#ifdef DEVICE_SYNTH_AUDUINO
#include "Auduino.h"  // eigene Wrapper-Klasse
Auduino softSynth;
#endif

// -----------------------------------------------------------------------------

#ifdef DEVICE_PCM
#include <PCM.h>                    // PCM samples
#endif // DEVICE_PCM

// -----------------------------------------------------------------------------

#ifdef DEVICE_SERIAL_IN

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

// -----------------------------------------------------------------------------

// TODO write serial handler/ callbacks (@see MIDI.h)

// -----------------------------------------------------------------------------

// setup
Serial.begin(38400);

// -----------------------------------------------------------------------------

// usage examle
Serial.println(F("Hello World!"));

#endif // DEVICE_SERIAL_IN

// -----------------------------------------------------------------------------

#ifdef DEVICE_MIDI

/** MIDI
* MIDI is a serial protocol that operates at 31,250 bits per second.
*
* MIDI-IN: clock, note on/ off, program change, CC, PB
* MIDI-OUT:
*
* https://www.arduino.cc/en/Tutorial/Midi
* https://github.com/FortySevenEffects/arduino_midi_library
*/

// -----------------------------------------------------------------------------

// Hardware
// use 220 ohm resistors on serial line and +5V-line

// -----------------------------------------------------------------------------

#include <SoftwareSerial.h>         // MIDI NOT via Rx (A0), Tx (A1) but any other definable port
#include <MIDI.h>                   // MIDI library

// -----------------------------------------------------------------------------

// Declaration, Definition, Initialization
SoftwareSerial midiSerial(PIN_MIDI_IN, PIN_MIDI_OUT);
//MIDI_CREATE_DEFAULT_INSTANCE();
MIDI_CREATE_INSTANCE(SoftwareSerial, midiSerial, MIDI);

// Declaration in a C++ class: no definition of static variables in class or constructor! write it somewhere in the cpp file.
SoftwareSerial DeviceMIDI::midiSerial(PIN_MIDI_IN, PIN_MIDI_OUT);
midi::MidiInterface<SoftwareSerial> DeviceMIDI::MIDI(midiSerial);

// -----------------------------------------------------------------------------

// setup
// register event listeners | event handler | callbacks
MIDI.setHandleClock(handleClock);
MIDI.setHandleStart(handleStart);
MIDI.setHandleStop(handleStop);
MIDI.setHandleContinue(handleContinue);
MIDI.setHandleNoteOn(handleNoteOn);
MIDI.setHandleNoteOff(handleNoteOff);
MIDI.setHandleControlChange(handleControlChange);
MIDI.setHandlePitchBend(handlePitchBend);
MIDI.setHandleProgramChange(handleProgramChange);

MIDI.begin(MIDI_CHANNEL_OMNI);

// -----------------------------------------------------------------------------

/* send messages */
void sendMIDI(int note, int vel, int ch) {
	MIDI.sendNoteOn(note, vel, ch);  // Send a Note (pitch 42, velo 127 on channel 1)
									 //delay(1000);		           // Wait for a second
									 //MIDI.sendNoteOff(note, 0, ch); // Stop the note
}

// -----------------------------------------------------------------------------

// CALLBACKS

/*
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

/** This function will be automatically called when a NoteOn is received.
* It must be a void-returning function with the correct parameters,
* see documentation here:
* http://arduinomidilib.fortyseveneffects.com/a00022.html
*/
void handleNoteOn(byte channel, byte pitch, byte velocity) {
	// Do whatever you want when a note is pressed.

	// Try to keep your callbacks short (no delays ect)
	// otherwise it would slow down the loop() and have a bad impact
	// on real-time performance.

	//unsigned int freq = mtof(pitch);
	// scale and use by DAC as CV

	MIDI.turnThruOff();
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
	// Do something when the note is released.
	// Note that NoteOn messages with 0 velocity are interpreted as NoteOffs.

	// @see libraries/MIDI/examples/MIDI_SimpleSynth
}

void handleControlChange(byte channel, byte number, byte value) {
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

void handlePitchBend(byte channel, int bend) {

}

void handleProgramChange(byte channel, byte number) {

}

void handleClock() {
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

void handleStart() {
	// reset all counter
	// start sequencer
}

void handleStop() {
	// stop all MIDI-Notes playing
	for (byte i = 0; i < 128; i++) // turn off all midi notes
		MIDI.sendNoteOff(i, 0, sequencer.midi.channel);
	// stop sequencer
}

void handleContinue() {

}

#endif // DEVICE_MIDI

// -----------------------------------------------------------------------------

#ifdef DEVICE_DAC

/** 
* CV/Gate
*/

// -----------------------------------------------------------------------------

// Hardware: Adafruit MCP4725 DAC via I2C, 12-bit
// For Adafruit MCP4725A0 the address is 0x60 or 0x61
// For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
// For Adafruit MCP4725A2 the address is 0x64 or 0x65
// use I2C-Adress 0x60 (110001)

// -----------------------------------------------------------------------------

#include <Wire.h>                   // alphanumeric Display, DAC
#include "Adafruit_MCP4725.h"       // DAC

// -----------------------------------------------------------------------------

// Declaration, Definition, Initialization
Adafruit_MCP4725 dac;

// -----------------------------------------------------------------------------

// setup
dac.begin(ADDR_DAC);

// -----------------------------------------------------------------------------

/**
* CAVE: very time consumung: 350 - 400ms
* (Better use LUT)
*
* @see Pure Data (PD)
* @see Mozzi
* https://github.com/vishnubob/armi/blob/master/firmware/mozzi/utils.cpp
*/
float mtof(byte midival) {
	// TODO dynamische Programmierung, use LUT
	return 8.1757989156 * pow(2.0, (float)midival / 12.0);
}

void sendCV(byte midiNoteNumber) {
	// 400ms, better use LUT (look up table)
	float frequency = mtof(midiNoteNumber);

	// map to 0..5 Volt
	// map(value, from_lower, from_higher, to_lower, to_higher);
	// map(frequency, ..., 0, 5);

	// DAC via I2C
	dac.setVoltage(frequency, false);
	//dac.setVoltage(frequency, false);
	return;
}

/*
+ @parameter - onOrOff: must be LOW or HIGH
*/
void sendGate(byte onOrOff) {
	digitalWrite(PIN_GATE, onOrOff);
	return;
}

#endif // DEVICE_DAC

// -----------------------------------------------------------------------------

#ifdef DEVICE_SHIFT_REGISTERS

/** SHIFT-REGISTER: LED-ARRAY
* 74HC595 serial to parallel shift registers
* http://bildr.org/2011/02/74hc595/
* http://arduino.cc/en/Tutorial/ShiftOut
*/

// -----------------------------------------------------------------------------

/** set all register pins to LOW */
void clearRegisters() {
	for (int i = NUM_REGISTER_PINS - 1; i >= 0; i--) {
		state.out.registers[i] = LOW;
	}
}

/** Set and display registers
* Only call AFTER all values are set how you would like (slow otherwise)
*/
void writeRegisters() {

	digitalWrite(PIN_SHIFT_RCLK, LOW);

	for (int i = NUM_REGISTER_PINS - 1; i >= 0; i--) {
		digitalWrite(PIN_SHIFT_SCLK, LOW);

		int val = state.out.registers[i];

		digitalWrite(PIN_SHIFT_SER, val);
		digitalWrite(PIN_SHIFT_SCLK, HIGH);

	}
	digitalWrite(PIN_SHIFT_RCLK, HIGH);

}

/** set an individual pin HIGH or LOW */
void setRegister(int index, int value) {
	state.out.registers[index] = value;
}

/**  */
void demoShift() {

	setRegister(2, HIGH);
	setRegister(3, HIGH);
	setRegister(4, LOW);
	setRegister(5, HIGH);
	setRegister(7, HIGH);


	writeRegisters();  //MUST BE CALLED TO DISPLAY CHANGES
					   //Only call once after the values are set how you need.
}

#endif // DEVICE_SHIFT_REGISTERS

// -----------------------------------------------------------------------------

#ifdef DEVICE_MUX

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

// -----------------------------------------------------------------------------

// Hardware: Pullup-Resistor for Buttons

// -----------------------------------------------------------------------------

// setup
// pullup for A0, A1 (?)

// -----------------------------------------------------------------------------

void getButtonState() {

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
			state.out.registers[OFFSET_MUX1 + i] = muxChannel[channel][i];
			state.out.registers[OFFSET_MUX2 + i] = muxChannel[channel][i];
		}
		writeRegisters();

		// read state
		if (channel < NUM_POTIS) {
			state.in.potis[channel] = analogRead(PIN_MUX2_SIG);
		}
		int value = analogRead(PIN_MUX1_SIG); // 10-bit values: 0..1023
											  // convert from analog 10-bit values to digital values: 0|1
		if (value > 512) { // treshold for noise
			currentstate[channel] = HIGH;
		}
		else {
			currentstate[channel] = LOW;
		}

		// DEBUG Debounce: every 3 sec
		if (DEBUG && (profilingLoops % 3000) == 0) {
			Serial.print("Read button_");
			Serial.print(channel, DEC);
			Serial.print(" value ");
			Serial.print(value, DEC);
			Serial.print(" to binary ");
			Serial.println(currentstate[channel], DEC);
		}

		// Debounce
		if (currentstate[channel] == previousstate[channel]) {
			if ((state.in.buttons.buttonstate[channel] == LOW) && (currentstate[channel] == LOW)) {
				// just pressed
				state.in.buttons.justpressed[channel] = 1;
			}
			else if ((state.in.buttons.buttonstate[channel] == HIGH) && (currentstate[channel] == HIGH)) {
				// just released
				state.in.buttons.justreleased[channel] = 1;
			}
			state.in.buttons.buttonstate[channel] = !currentstate[channel];  // remember, digital HIGH means NOT pressed
		} // end if
		previousstate[channel] = currentstate[channel];
	} // end for channel
} // end getButtonStates()

#endif // DEVICE_MUX

  // -----------------------------------------------------------------------------

#ifdef DEVICE_ENCODER

  /** Rotary-Encoder
  * Encoder + Pushbutton + RGB-LED
  * https://www.circuitsathome.com/mcu/programming/reading-rotary-encoder-on-arduino
  * http://www.circuitsathome.com/mcu/rotary-encoder-interrupt-service-routine-for-avr-micros
  * https://code.google.com/p/m2tklib/wiki/t09
  */

  // -----------------------------------------------------------------------------

  /**
  * Read Encoder Change
  * @see https://de.wikipedia.org/wiki/Bitfeld#Bit_auslesen
  * @return change in encoder state (-1,0,1)
  */
int8_t readEncoder() {
	static int8_t enc_states[] = { 0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0 };
	static uint8_t old_AB = 0;
	old_AB <<= 2;                   //remember previous state
	old_AB |= (ENC_PORT & 0x03);  //add current state
	return (enc_states[(old_AB & 0x0f)]);
}

/**
* Sets the Color and Brightness of the RGB-LED
* with PWM
*
* @param byte red   (0..255)
* @param byte green (0..255)
* @param byte blue  (0..255)
*/
void setRGBLED(byte r, byte g, byte b) {
	// set 3 PWM PINs
	analogWrite(PIN_ENCODER_LED_R, r);
	analogWrite(PIN_ENCODER_LED_G, g);
	analogWrite(PIN_ENCODER_LED_B, b);

}

/***/
void demoRotary() {
	static uint8_t counter = 0;      //this variable will be changed by encoder input
	int8_t value;
	value = readEncoder();
	if (value) {
		Serial.print("Counter value: ");
		Serial.println(counter, DEC);
		counter += value;
	}
}

#endif // DEVICE_ENCODER

// -----------------------------------------------------------------------------

/* LED */
/* Examples */

static const int ledPin = 13;                  // the number of the LED pin
static int ledState = LOW;                      // ledState used to set the LED

												// Generally, you should use "unsigned long" for variables that hold time
												// The value will quickly become too large for an int to store
												//static unsigned long previousMillis = 0;        // will store last time LED was updated

												// constants won't change :
												//static const long interval = 1000;  

												//static unsigned long currentMillis = millis();

void blink(long interval) {

	if (currentMillis - previousMillis >= interval) {
		// save the last time you blinked the LED 
		previousMillis = currentMillis;

		// if the LED is off turn it on and vice-versa:
		if (ledState == LOW)
			ledState = HIGH;
		else
			ledState = LOW;

		// set the LED with the ledState of the variable:
		digitalWrite(ledPin, ledState);
	}
}

// -----------------------------------------------------------------------------

#ifdef DEVICE_NEOPIXELS

/** Neopixels
* strip ws2812 with 32 RGB-pixels
*/

// -----------------------------------------------------------------------------

// Hardware: ws2812 LED-Strip (Adafruit "Neopixels")
// use external power supply (5V, 60mAh per pixel): max. 1920mAh (10W) for 32 pixels
// use 1000uF capacitor across power-(+/-)-lines
// use 470 ohm Resistor for data-line
// always connect ground first on assembly (disconnect ground last on disassembly)

// -----------------------------------------------------------------------------

#include "Adafruit_NeoPixel.h"      // Adafruit Neopixels

// -----------------------------------------------------------------------------

// Declaration, Definition, Initialization
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN_NEO, NEO_GRB + NEO_KHZ800);

// -----------------------------------------------------------------------------

// setup
strip.begin();
strip.show(); // set all pixels to 'off'

// -----------------------------------------------------------------------------

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
	if (WheelPos < 85) {
		return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	}
	else if (WheelPos < 170) {
		WheelPos -= 85;
		return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	else {
		WheelPos -= 170;
		return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
	for (uint16_t i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, c);
		strip.show();
		delay(wait);
	}
}

void rainbow(uint8_t wait) {
	uint16_t i, j;

	for (j = 0; j < 256; j++) {
		for (i = 0; i < strip.numPixels(); i++) {
			strip.setPixelColor(i, Wheel((i + j) & 255));
		}
		strip.show();
		delay(wait);
	}
}

#endif // DEVICE_NEOPIXELS


// -----------------------------------------------------------------------------

#ifdef DEVICE_DISPLAY

/**
* Alphanumeric Display
* 0.54", 14 segments, 4 letters
* I2C (Chip on PCB)
* https://learn.adafruit.com/adafruit-led-backpack/0-54-alphanumeric
* File > Examples > Adafruit_LEDBackpack > quadalphanum
*/

// -----------------------------------------------------------------------------

// Hardware: Adafruit Alphanumeric Display 0.54", 14 segments, 4 letters
// I2C-adress is 0x70 by default

// -----------------------------------------------------------------------------

#include "Wire.h"                   // alphanumeric Display, DAC
#include "Adafruit_LEDBackpack.h"   // alphanumeric Display
#include "Adafruit_GFX.h"           // alphanumeric Display

// -----------------------------------------------------------------------------

// Declaration, Definition, Initialization
Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

// -----------------------------------------------------------------------------

// setup
alpha4.begin(ADDR_ALPHA);

// -----------------------------------------------------------------------------

/* TODO: This is Blocking Code ! */
void alphaWrite(char *text) {

	char displaybuffer[4] = { ' ', ' ', ' ', ' ' };

	for (int i = 0; i<sizeof(text); i++) {
		char c = text[i];

		// scroll down display
		displaybuffer[0] = displaybuffer[1];
		displaybuffer[1] = displaybuffer[2];
		displaybuffer[2] = displaybuffer[3];
		displaybuffer[3] = c;

		// set every digit to the buffer
		alpha4.writeDigitAscii(0, displaybuffer[0]);
		alpha4.writeDigitAscii(1, displaybuffer[1]);
		alpha4.writeDigitAscii(2, displaybuffer[2]);
		alpha4.writeDigitAscii(3, displaybuffer[3]);

		// write it out!
		alpha4.writeDisplay();
		delay(200);
	}
}

/* Demo */
/* TODO: This is Blocking Code ! */
void alphaReadSerial() {
	char displaybuffer[4] = { ' ', ' ', ' ', ' ' };

	while (!Serial.available()) return;

	char c = Serial.read();
	if (!isprint(c)) return; // only printable!

							 // scroll down display
	displaybuffer[0] = displaybuffer[1];
	displaybuffer[1] = displaybuffer[2];
	displaybuffer[2] = displaybuffer[3];
	displaybuffer[3] = c;

	// set every digit to the buffer
	alpha4.writeDigitAscii(0, displaybuffer[0]);
	alpha4.writeDigitAscii(1, displaybuffer[1]);
	alpha4.writeDigitAscii(2, displaybuffer[2]);
	alpha4.writeDigitAscii(3, displaybuffer[3]);

	// write it out!
	alpha4.writeDisplay();
	delay(200);
}

#endif // DEVICE_DISPLAY

// -----------------------------------------------------------------------------

#ifdef DEVICE_TRELLIS

/** Adafruit Trellis
* Button-Array + LED-Array using I2C
* https://learn.adafruit.com/adafruit-trellis-diy-open-source-led-keypad/overview
* https://github.com/adafruit/Adafruit_Trellis_Library
* Examples
* https://learn.adafruit.com/mini-untztrument-3d-printed-midi-controller
* https://learn.adafruit.com/trellis-3d-printed-enclosure
*
* Changing Addresses: You can change the address of very easily. Look on the back
to find the three A0, A1 or A2 solder jumpers. Each one of these is used to 
hardcode in the address. If a jumper is shorted with solder, that sets the address. 
A0 sets the lowest bit with a value of 1, A1 sets the middle bit with a value of 2 
and A2 sets the high bit with a value of 4. The final address is 0x70 + A2 + A1 + A0. 
So for example if A2 is shorted and A0 is shorted, the address is 0x70 + 4 + 1 = 0x75. 
If only A1 is shorted, the address is 0x70 + 2 = 0x72
*/

// -----------------------------------------------------------------------------

// Hardware: Adafruit Trellis
// use I2C-adress 0x72 (shorten A1)

// -----------------------------------------------------------------------------

#include "Adafruit_Trellis.h"       // Adafruit Trellis

//#define INTPIN				A3			// Connect the INT wire to pin #A2 (can change later!)

// -----------------------------------------------------------------------------

// Declaration, Definition, Initialization
Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_TrellisSet trellis = Adafruit_TrellisSet(&matrix0);

// -----------------------------------------------------------------------------

// setup
trellis.begin(ADDR_TRELLIS);

// -----------------------------------------------------------------------------

/***/
void trellisReadButtons() {
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
void trellisWrite(byte pattern[]) {
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

void trellisDemo() {
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

#endif // DEVICE_TRELLIS


void setup() {
	// PINs --------------------------------------------------------------------

	
	// USB-Serial --------------------------------------------------------------
	Serial.begin(38400);
	Serial.println("Start Synth_Workstation:");

#ifdef DEVICE_MIDI
	// MIDI is a serial protocol that operates at 31,250 bits per second
	// register event listeners | event handler | callbacks
	MIDI.setHandleClock(handleClock);
	MIDI.setHandleStart(handleStart);
	MIDI.setHandleStop(handleStop);
	MIDI.setHandleContinue(handleContinue);
	MIDI.setHandleNoteOn(handleNoteOn);
	MIDI.setHandleNoteOff(handleNoteOff);
	MIDI.setHandleControlChange(handleControlChange);
	MIDI.setHandlePitchBend(handlePitchBend);
	MIDI.setHandleProgramChange(handleProgramChange);

	MIDI.begin(MIDI_CHANNEL_OMNI);
#endif

#ifdef DEVICE_SYNTH_AUDUINO
	softSynth = Auduino();
#endif

#ifdef DEVICE_SHIFT_REGISTERS
	//reset all register pins
	clearRegisters();
	writeRegisters();
#endif

#ifdef DEVICE_MUX
#endif

#ifdef DEVICE_ENCODER
#endif

#ifdef DEVICE_NEOPIXELS
	strip.begin();
	strip.show(); // set all pixels to 'off'
#endif

#ifdef DEVICE_DAC
	dac.begin(ADDR_DAC);
#endif

#ifdef DEVICE_DISPLAY
	alpha4.begin(ADDR_ALPHA);
#endif

#ifdef DEVICE_TRELLIS
	trellis.begin(ADDR_TRELLIS);
#endif

}


void loop() {}


#endif
