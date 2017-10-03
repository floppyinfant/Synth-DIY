﻿/** ***************************************************************************
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
// @see Device*.h

// device Arduino -> PINs ------------------------------------------------------

#define PIN_SERIAL_RX          0    // D0 == Rx
#define PIN_SERIAL_TX          1    // D1 == Tx
#define PIN_MIDI_IN           12    // or D0 == Rx
#define PIN_MIDI_OUT          13    // or D1 == Tx; D13 == LED
#define PIN_GATE              A0    // TODO or use this PIN for Neopixel, Ribbon-Controller
#define PIN_AUDUINO_PWM        3
#define PIN_AUDUINO_LED       13    // TODO: CAVE PIN 13 is MIDI-OUT

#define PIN_SHIFT_SER          2    // data;  Serial
#define PIN_SHIFT_SCLK         4    // clock; Serial Clock
#define PIN_SHIFT_RCLK         7    // latch; Register Clock

#define PIN_MUX1_SIG          A1    // INPUT, digital
#define PIN_MUX2_SIG          A2    // INPUT, analog

#define PIN_ENCODER_A          8
#define PIN_ENCODER_B          9
#define PIN_ENCODER_PUSH      10
#define PIN_ENCODER_LED_R      5    // PWM
#define PIN_ENCODER_LED_G      6    // PWM
#define PIN_ENCODER_LED_B     11    // PWM

#define PIN_JOY_X             A6
#define PIN_JOY_Y             A7
// conflicting PINs: not used yet
#define PIN_JOY_P             11    // TODO Joystick Push

#define PIN_NEO               10    // TODO PWM, Dn~; PIN used by Encoder Push at the moment


// I2C -------------------------------------------------------------------------
// Connect I2C SDA pin to your Arduino SDA line (A4, 22)
// Connect I2C SCL pin to your Arduino SCL line (A5, 21)

#define ADDR_DAC            0x60    //1100010 or 1100011 (if select pin A0 is high)
#define ADDR_ALPHA          0x70    //1110000
#define ADDR_TRELLIS        0x72    //set jumper A1; 1110000 - 1110111 (select jumpers A0,A1,A2)


// device EEPROM ---------------------------------------------------------------


// device PROGMEM --------------------------------------------------------------


// device SerialIn -------------------------------------------------------------


// device MIDI-In, MIDI-Out ----------------------------------------------------


// device DAC -> CV + Gate -----------------------------------------------------
//#define DAC_RESOLUTION         8    // Set this value to 9 (512 values), 8 (256 values), 7 (128 values), 6 (64 values) or 5 (32 values) to adjust the resolution


// device Auduino -> Audio out -------------------------------------------------


// device ShiftRegister (x3) ---------------------------------------------------
// shift registers
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


// device Encoder -> Encoder, RGB_LED ------------------------------------------


// device Neopixels ------------------------------------------------------------
#define NUM_PIXELS            32


// device Alphanumeric Display (x2) --------------------------------------------
#define NUM_DIGITS             4    // TODO: alphanum display number of digits


// device Trellis -> Buttons, LEDs ---------------------------------------------
#define NUM_TRELLIS            1
#define NUM_TRELLIS_BUTTONS    (NUM_TRELLIS * 16)

#define MOMENTARY              0
#define LATCHING               1
#define MODE                   LATCHING    // set the mode here

// UI (Trellis Buttons)
#define BTN_REC                0
#define BTN_PLAY               1
#define BTN_STOP               1    // same button as PLAY
#define BTN_PREV               2
#define BTN_NEXT               3
#define BTN_NOTES              4    // chromatic scale
#define BTN_MAJOR              5    // DUR circle of quints
#define BTN_MINOR              6    // moll circle of quints
#define BTN_STEP_OR_NOTE       7    // pushed (on) means step
// root position, if none of the next two is selected
#define BTN_FIRST_INVERSION    8    // chord inversion
#define BTN_SECOND_INVERSION   9    // chord inversion
// prime, of none of the next two buttons is selected
#define BTN_REVERSE           10    // retrograde
#define BTN_INVERSE           11    // inversion
#define BTN_SAVE              12
#define BTN_LOAD_OR_EDIT      13
#define BTN_SETTINGS          14
#define BTN_SEQ_LENGTH        15


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

/* STATE ******************************************************************** */

extern struct state_t {
	struct in {
		struct buttons {
			byte buttonstate[NUM_BUTTONS];
			byte justpressed[NUM_BUTTONS];
			byte justreleased[NUM_BUTTONS];
		} buttons;
		int potis[NUM_POTIS];    // analog data is 10 bits
		byte trellis[16];
		byte encoder;

		struct arduino {
			struct joy {
				byte x;
				byte y;
				byte pushed;
			} joy;

			byte ribbon;
			// more?

		} arduino;
	} in;

	struct out {
		byte registers[NUM_REGISTER_PINS];    // 3 shift registers
		byte buttonLEDs[16];
		byte RGBLED[3];
		char alphanumDisplay[NUM_DIGITS];
		byte trellis[16];
		byte neopixels[32];
	} out;
} state;


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
