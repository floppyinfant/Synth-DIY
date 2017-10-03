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

// -----------------------------------------------------------------------------
/* HEADERS */
// -----------------------------------------------------------------------------

#include "Workstation.h"      // Constants, global variables, Prototypes
#include "Devices.h"
#include "Sequencer.h"
#include "scales.h"           // LUT, Scales + Chords, Quintenzirkel, Arpeggio-Patterns, Rhythm-Patterns, Styles
//#include "pitches.h"		  // from libraries/MIDI/MIDI_SimpleSynth


// -----------------------------------------------------------------------------
/* GLOBAL VARIABLES */
// -----------------------------------------------------------------------------

// PROFILING

long profilingTime;
long profilingLoops;


// UI
int menu = MENU_MAIN;
int mode = MODE_PLAY;


// SEQUENCER
long clock; // actual time
long step;  // actual step
long nextStep;
int timePerStep; // getTimeFromBpm(tempo) / 16;
boolean stepPlayed;


// TIMER
// state machine
// @see SimpleTimer.h
// @see Interrupts
// @see Adafruit
unsigned long currentMillis = millis();
unsigned long previousMillis = 0;

unsigned long time = micros(); // millis()
unsigned long timer1;
unsigned long timer2;
unsigned long timer3;


// structs
struct state_t state;
struct sequencer_t sequencer;

// Devices
Devices hw;


// -----------------------------------------------------------------------------
/* Memory */
// -----------------------------------------------------------------------------

// EEPROM (1kB)
// @see Devices.h


// -----------------------------------------------------------------------------
/* DEVICES */
// -----------------------------------------------------------------------------

// @see Devices.h, Device....h


// -----------------------------------------------------------------------------

/* LED */
/* Examples */

static const int ledPin =  13;                  // the number of the LED pin
static int ledState = LOW;                      // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
//static unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change :
//static const long interval = 1000;  

//static unsigned long currentMillis = millis();

void blink(long interval) {
	
	if(currentMillis - previousMillis >= interval) {
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
/* SETUP | INITIALIZATIONS */
// -----------------------------------------------------------------------------

//void setup() {
void initHardware() {

	// PINs --------------------------------------------------------------------
	// TODO: pullup resistors for switches

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

    /*
    // Make input & enable pull-up resistors on switch pins
    for (i=0; i< NUMBUTTONS; i++) {
      pinMode(buttons[i], INPUT);
      digitalWrite(buttons[i], HIGH);
    }*/

    
	// USB-Serial --------------------------------------------------------------
	// MIDI is a serial protocol that operates at 31,250 bits per second.

    Serial.begin(115200);
    Serial.println("Start Synth_Workstation:");

	// load settings from EEPROM -----------------------------------------------
    //loadSettings();

    
	// INITIALIZATION of structs -----------------------------------------------

	// state -------------------------------------------------------------------
	struct state_t state = {
        {0}, // in
        {0}  // out
    };

    // Sequencer ---------------------------------------------------------------
    struct sequencer_t sequencer = {0};


} // end setup


// -----------------------------------------------------------------------------
/* LOOP */
// -----------------------------------------------------------------------------

//void loop() {
void run() {

	if (TEST) {

		//test();

	} else {
		// normal operating mode

		// DEMO and TESTS //////////////////////////////////////////////////////////
		Serial.println("Hello Workstation");

		//hw.devDisplay.alphaReadSerial;

        // -------------------------------------------------------------------------
        // 1) Input: MIDI-In-Port abfragen
        //    a) clock
        //    b) note-on
        //    @see MIDI-Implementation-Chart



        // -------------------------------------------------------------------------
        // 2) Input: Sensoren abfragen
        //    a) MUX <- Buttons, Potis
        //    b) Encoder
        //    c) Trellis Button-Array (I2C)
        //    d) analogRead() <- Joystick

		//getButtonState();
		//getEncoderChange();
		//getTrellisButtonState();
		//getArduinoInputs();
		
        // -------------------------------------------------------------------------
        // 3) Menue (?)
        //    a) ARPEGGIATOR Steuerung
        //    b) save state to EEPROM when a setting changes

		//updateMenu();

        // -------------------------------------------------------------------------
        // 4) Update Sequencer State
        //    a) clock and time
        //    b) mode

		//updateSequencer();

        // -------------------------------------------------------------------------
        // 5) Sequencer Output
        //    a) MIDI-Out
        //    b) DAC <- I2C (CV) + Gate



        // -------------------------------------------------------------------------
        // 6) Output: Displays, LEDs
        //    a) alphanum Display <- I2C
        //    b) Button Array <- ShiftRegister
        //         just set the bits; shiftOut() is done in ReadMUX()
        //    c) RGB-LED (Encoder)
        //         shows the beats on PLAY (blue), REC (red), STEP: MIDI-IN (green), STOP (rainbow)
        //    d) Trellis Array <- I2C
        //    e) Neopixels

		//updateViews();
		
	} // end normal operating mode
	
} // end loop
