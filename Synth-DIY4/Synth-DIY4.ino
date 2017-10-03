/* ***************************************************************************** 
<filename>

<short_description>

<long_description><references>

<copyright>
Copyright (c) 2017, Totman
All rights reserved.

<license>
https://en.wikipedia.org/wiki/Software_license
https://en.wikipedia.org/wiki/Comparison_of_free_and_open-source_software_licenses
https://opensource.org/licenses
https://choosealicense.com/licenses/
https://tldrlegal.com/

<disclaimer>
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

<author+contact> <date> <version>

<version_history>

 **************************************************************************** */

// -----------------------------------------------------------------------------
// TODO
// -----------------------------------------------------------------------------

// --------------------------------
// DOXYGEN
/*!
@file     <filename.cpp>
@author   <name, company>
@license  <license>
@section  HISTORY
@brief    <description>
@param[in]
*/
// --------------------------------

// --------------------------------
// Github, Website, Marketing, Community (Tutorial, Wiki, Forum), Support, Monetizing (Donations)
// --------------------------------

// -----------------------------------------------------------------------------
// PROGRAMMING PARDIGMS
// -----------------------------------------------------------------------------
// state machine
//
// Multitasking (Timers, Interrupts, millis())
// https://learn.adafruit.com/multi-tasking-the-arduino-part-1/overview
// https://learn.adafruit.com/multi-tasking-the-arduino-part-2/overview
// https://learn.adafruit.com/multi-tasking-the-arduino-part-2/timers
// https://learn.adafruit.com/multi-tasking-the-arduino-part-3/overview
//
// https://playground.arduino.cc/Main/LibraryList#Timing


// -----------------------------------------------------------------------------
// header, includes
// -----------------------------------------------------------------------------
#include "settings.h"
#include "DeviceLED.h"

/*
#include "DeviceNeopixels.h" // ecapsulate higher level abstractions: e.g. show beat 4, step 2; animate(color, direction, speed, blink | fade)
#include "DeviceMIDI.h"

#include "DeviceDAC.h"
#include "DeviceDisplay.h"
#include "DeviceTrellis.h" // has a DeviceButton (has a DeviceLED) ???

#include "DeviceEncoder.h" // has a DeviceLED::type == RGBLED

#include "DeviceShiftRegister.h" // has n DeviceButton (has a DeviceLED)
*/

// -----------------------------------------------------------------------------
// definitions, constants
// -----------------------------------------------------------------------------
#define PIN_SERIAL_RX          0    // D0 == Rx
#define PIN_SERIAL_TX          1    // D1 == Tx
#define PIN_MIDI_IN           12    // or D0 == Rx
#define PIN_MIDI_OUT          13    // or D1 == Tx; D13 == LED
#define PIN_GATE              A0    // TODO or use this PIN for Neopixel, Ribbon-Controller
#define PIN_AUDUINO_PWM        3
#define PIN_AUDUINO_LED       13    // TODO: CAVE PIN 13 is MIDI-OUT
#define PIN_MUX1_SIG          A1    // INPUT, digital
#define PIN_MUX2_SIG          A2    // INPUT, analog
#define PIN_ENCODER_A          8
#define PIN_ENCODER_B          9
#define ENC_PORT            PINB    //PINC for 14 (A0) + 15 (A1); PINB for 8 + 9; PIND for 0 + 1
#define PIN_ENCODER_PUSH      10
#define PIN_ENCODER_LED_R      5    // PWM
#define PIN_ENCODER_LED_G      6    // PWM
#define PIN_ENCODER_LED_B     11    // PWM
#define PIN_JOY_X             A6
#define PIN_JOY_Y             A7
// conflicting PINs: not used yet
#define PIN_JOY_P             11    // TODO Joystick Push
#define PIN_NEO               10    // TODO PWM, Dn~; PIN used by Encoder Push at the moment

#define PIN_SHIFT_SER          2    // data;  Serial
#define PIN_SHIFT_SCLK         4    // clock; Serial Clock
#define PIN_SHIFT_RCLK         7    // latch; Register Clock

// I2C
// Connect I2C SDA pin to your Arduino SDA line (A4, 22)
// Connect I2C SCL pin to your Arduino SCL line (A5, 21)
#define ADDR_DAC            0x60    //1100010 or 1100011 (if select pin A0 is high)
#define ADDR_ALPHA          0x70    //1110000
#define ADDR_TRELLIS        0x72    //set jumper A1; 1110000 - 1110111 (select jumpers A0,A1,A2)


// -----------------------------------------------------------------------------
// Makros
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// type declarations, typedef
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// extern (global) variables
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// prototypes, function declarations
// -----------------------------------------------------------------------------
// end of header file ----------------------------------------------------------
// -----------------------------------------------------------------------------
// function definitions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// global variables (definition of extern variables)
// -----------------------------------------------------------------------------
//Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();
static String command = String(32);
DeviceLED devLed13 = DeviceLED(13);  // DEMO
DeviceLED devRgbLed = DeviceLED(PIN_ENCODER_LED_R, PIN_ENCODER_LED_G, PIN_ENCODER_LED_B);


void setup() {
	// -------------------------------------------------------------------------
	// PINs
	// -------------------------------------------------------------------------
	// TODO: pullup resistors for switches

	pinMode(PIN_AUDUINO_PWM, OUTPUT);
	//pinMode(PIN_AUDUINO_LED, OUTPUT); // is PIN 13, which is also used for MIDI-OUT

	pinMode(PIN_ENCODER_A, INPUT);
	pinMode(PIN_ENCODER_B, INPUT);
	pinMode(PIN_ENCODER_PUSH, INPUT);
	digitalWrite(PIN_ENCODER_A, HIGH);
	digitalWrite(PIN_ENCODER_B, HIGH);

	pinMode(PIN_ENCODER_LED_R, OUTPUT);
	pinMode(PIN_ENCODER_LED_G, OUTPUT);
	pinMode(PIN_ENCODER_LED_B, OUTPUT);
	digitalWrite(PIN_ENCODER_LED_R, LOW);  // set defined state
	digitalWrite(PIN_ENCODER_LED_G, LOW);  // set defined state
	digitalWrite(PIN_ENCODER_LED_B, LOW);  // set defined state

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

	
	// -------------------------------------------------------------------------
	// Instantiations
	// -------------------------------------------------------------------------


	// -------------------------------------------------------------------------
	// Initialisations, "begin"
	// -------------------------------------------------------------------------

	// --------------------------------
	// serial console
	// --------------------------------
	Serial.begin(115200);
	Serial.println("Synthesizer Workstation started!");


	// -------------------------------------------------------------------------
	// load settings
	// -------------------------------------------------------------------------

}

void loop() {
	// -------------------------------------------------------------------------
	// INPUT
	// -------------------------------------------------------------------------

	// --------------------------------
	// Serial Remote
	//
	// https://playground.arduino.cc/Code/SerialControl
	// https://github.com/UrsusExplorans/SerialControl/
	//
	// https://www.arduino.cc/en/Reference/StringObject
	// https://www.arduino.cc/en/Tutorial/CharacterAnalysis
	// --------------------------------

	if (Serial.available() > 0) {
		char c = Serial.read();
		//int c = Serial.parseInt();
    Serial.print(c);
		if (c =! '\n') { // newline, ASCII 10
			command += c;
			//Serial.print(c);
		}
		else {
			command += "\n";
			String s = "received command (";
			s += sizeof(command);
			s += "b): ";
			s += command;
			Serial.println(s);
			// reset command
			command = "";
		}
	}

	// --------------------------------
	// MIDI-IN
	// --------------------------------

	// --------------------------------
	// Encoder
	// --------------------------------

	// --------------------------------
	// Trellis Buttons
	// --------------------------------

	// --------------------------------
	// ShiftRegister --> MUX1+2 --> ButtonArray | PotiArray
	// --------------------------------

	// --------------------------------
	// analog Inputs: Joystick
	// --------------------------------


	// -------------------------------------------------------------------------
	// UPDATE
	// -------------------------------------------------------------------------

	// --------------------------------
	// State
	// EEPROM
	// --------------------------------

	// --------------------------------
	// Menu
	// --------------------------------

	// --------------------------------
	// Sequencer
	// MIDI-OUT
	// CV/ Gate
	// Audio | Synthesizer (DAC, PCM|I2S, PWM)
	// --------------------------------


	// -------------------------------------------------------------------------
	// OUTPUT
	// -------------------------------------------------------------------------

	// --------------------------------
	// alphanum Display
	// --------------------------------

	// --------------------------------
	// Trellis
	// --------------------------------

	// --------------------------------
	// ButtonArray-LEDArray --> ShiftRegister
	// --------------------------------

	// --------------------------------
	// LED (builtin)
	// --------------------------------
	devLed13.blink(2000);  // DEMO

	// --------------------------------
	// RGB-LED
	// --------------------------------

	// --------------------------------
	// Neopixels
	// --------------------------------


}
