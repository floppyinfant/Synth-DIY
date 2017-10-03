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
 * @license Public Domain | CC | (L)GPL | Apache | BSD | MIT | Proprietary
 * https://tldrlegal.com/
 * Open Source, Open Hardware
 *
 * Copyright by Thorsten Mauthe
 * floppyinfant.com - all right reserved
 *
 *****************************************************************************/

// architecture pattern: state machine (non-blocking code without delay() but millis() and timers)
// @see Adafruit https://learn.adafruit.com/multi-tasking-the-arduino-part-1


#ifndef SYNTH_WORKSTATION_H
#define SYNTH_WORKSTATION_H


/* *****************************************************************************
 * HEADERS
 * ************************************************************************** */

/* User */
#include "settings.h"

/* Arduino */
#include "Arduino.h"

/* Arduino Library */
#ifdef DEVICE_EEPROM
#include <EEPROM.h>                 //for reading and writing patterns to EEPROM
#endif
#ifdef DEVICE_MIDI
#include <SoftwareSerial.h>         // MIDI NOT via Rx (A0), Tx (A1) but any other definable port
#include <MIDI.h>                   // MIDI library
#endif
#ifdef DEVICE_SYNTH_AUDUINO
#include "Auduino.h"
#endif
#ifdef DEVICE_PCM
#include <PCM.h>                    // PCM samples
#endif
#ifdef DEVICE_DAC
#include <Wire.h>                   // alphanumeric Display, DAC
#include "Adafruit_MCP4725.h"       // DAC
#endif
#ifdef DEVICE_NEOPIXELS
#include "Adafruit_NeoPixel.h"      // Adafruit Neopixels
#endif
#ifdef DEVICE_DISPLAY
//#include <Wire.h>                   // alphanumeric Display, DAC
#include "Wire.h"                   // alphanumeric Display, DAC
#include "Adafruit_LEDBackpack.h"   // alphanumeric Display
#include "Adafruit_GFX.h"           // alphanumeric Display
#endif
#ifdef DEVICE_TRELLIS
#include "Adafruit_Trellis.h"       // Adafruit Trellis
#endif

#include <Bounce2.h>                // Debouncing for button presses
//#include <SimpleTimer.h>            // 

/* Atmel AVR ATmega328P */
/* AVR-libc */
// http://www.nongnu.org/avr-libc/user-manual/index.html
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>           // included so we can use PROGMEM

/* Standard C Library */
//#include <stddef.h>
#include <stdlib.h>                 // string to numbers: atoi, atol, atof, sprintf; malloc, ...
//#include <stdio.h>                // I/O: fopen, fclose, fseek, fflush, fgetc, fputc, fgets, fputs, fread, fwrite, scanf, printf (f=file pointer, c=char, s=string, f=formatstring, v=vector)
//#include <string.h>               // strcat, strcmp, strcpy, strlen, etc.
//#include <ctype.h>                // char-type testen | umwandeln: isalpha, isdigit, ..., toupper, tolower
//#include <math.h>

/* Standard C++ Library */
/* STL (Standard Template Library) */


/* *****************************************************************************
 * Constants
 * ************************************************************************** */

// #define ONE 1

// @see Sequencer.h
// @see Hardware.h


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


/* HARDWARE */
// @see Workstation.cpp (global space)

//#ifdef DEVICE_...
//#endif


/* STATE */
// @see Hardware.h


/* SEQUENCER */
// @see Sequencer.h


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


/* Device Arduino */
//void setup(void);
void initHardware(void);
//void loop(void);
void run(void);


// TODO: encapsulate functions as class members --------------------------------

/* Hardware */
void getSerialIn(void);
void getButtonState(void);
void getEncoderChange(void);
void getTrellisButtonState(void);
void getArduinoInputs();

void blink(long);

/* UI */
void updateViews(void);
void updateMenu(void);

/* Sequencer */
void updateSequencer(void);

void startSequencer(void);
void stopSequencer(void);
void pauseSequencer(void);
void playSequence(void);
void recordSequence(void);

void sendNoteOn(byte);
void sendCVGate(byte);
void sendSynth(byte);


/* Helpers */
void log(char*);
float getTimePerStepFromBpm(int);

#ifdef DEVICE_EEPROM
// device EEPROM
void loadSettings(void);
void saveSettings(void);
#endif

#ifdef DEVICE_PCM
// device PROGMEM
void playSample(void);
#endif

// device SerialIn

#ifdef DEVICE_MIDI
// device MIDI-In, MIDI-Out
void handleClock(void);
void handleStart(void);
void handleStop(void);
void handleContinue(void);
void handleNoteOn(byte, byte, byte);
void handleNoteOff(byte, byte, byte);
void handleControlChange(byte, byte, byte);
void handlePitchBend(byte, int);
void handleProgramChange(byte, byte);

void sendMIDI(int, int, int);
#endif

#ifdef DEVICE_DAC
// device DAC -> CV + Gate
float mtof(byte);
void sendCV(int);
void sendGate(byte);
#endif

#ifdef DEVICE_SYNTH_AUDUINO
// device Auduino -> Audio out
void initAuduino(void);
void demoAuduino(void);
void audioOn(void);
//SIGNAL(PWM_INTERRUPT);
#endif

#ifdef DEVICE_SHIFT_REGISTERS
// device ShiftRegister (x3)
void clearRegisters(void);
void writeRegisters(void);
void setRegister(int, int);
void demoShift(void);
#endif

#ifdef DEVICE_MUX
// device MUX (x2) -> buttons[], potis[], PIN_ANALOG_IN_...
void getButtonState(void);
#endif

#ifdef DEVICE_ENCODER
// device Encoder -> Encoder, RGB_LED
int8_t readEncoder(void);
void setRGBLED(byte, byte, byte);
void demoRotary(void);
#endif

#ifdef DEVICE_NEOPIXELS
// device Neopixels
uint32_t Wheel(byte);
void colorWipe(uint32_t, uint8_t);
void rainbow(uint8_t);
#endif

#ifdef DEVICE_DISPLAY
// device Alphanumeric Display (x2)
void alphaWrite(char *);
void alphaReadSerial(void);
#endif

#ifdef DEVICE_TRELLIS
// device Trellis -> Buttons, LEDs
void trellisReadButtons(void);
void trellisWrite(byte[]);
void trellisDemo(void);
#endif


/* TESTS */
// @see Tests.h

#endif // SYNTH_WORKSTATION_H
