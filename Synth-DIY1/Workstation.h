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


// My Definitions
#include "settings.h"

// Arduino
#include "Arduino.h"

// Atmel AVR ATmega328P
// AVR-libc
// http://www.nongnu.org/avr-libc/user-manual/index.html
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>           // included so we can use PROGMEM


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


// Standard C Library
//#include <stddef.h>
#include <stdlib.h>                 // string to numbers: atoi, atol, atof, sprintf, etc.; malloc, etc.
//#include <stdio.h>                // I/O: fopen, fclose, fseek, fflush, fgetc, fputc, fgets, fputs, fread, fwrite, scanf, printf (f=file pointer, c=char, s=string, f=formatstring, v=vector)
//#include <string.h>               // strcat, strcmp, strcpy, strlen, etc.
//#include <ctype.h>                // char-type testen | umwandeln: isalpha, isdigit, ..., toupper, tolower
//#include <math.h>


/* *****************************************************************************
 * Constants
 * ************************************************************************** */

// Sequencer -------------------------------------------------------------------

#define NUM_POLYPHON           8    // Chord-Memory for Arpeggiator
#define NUM_SONGS              1
#define NUM_SONG_LENGTH        1    // song positions to store pattern numbers to play in sequence
#define NUM_TRACKS             1    // multitimbral: num voices (Melody|Lead-Harmony|Rhythm-Synth2-Bass-DrumKit:Cymbals|HH-Snare-TomToms|Percussion-Bassdrum)
#define NUM_STEPS             32    // array length = 4 steps * 4 beats * 2 Measures (=32 beats) * 8 Parts (Intro-Main-Fill_AB-Main2-Fill_BA-Var1-Var2-End) = 256
#define NUM_PATTERN            1
// TOTAL_MEMORY = NUM_STEPS * NUM_TRACKS * NUM_PATTERN

// UI (frontend)
#define MENU_MAIN              0
#define MENU_SET_PREFERENCES   1
#define MENU_SET_SEQUENCER     2
#define MENU_SET_ARPEGGIATOR   3
#define MENU_SET_VARIATION     4
#define MENU_SET_TRANSPOSITION 5
//enum Menu {MAIN,SET_CLOCK,SET_SEQUENCE,SET_TRANSPOSITION,SET_VARIATION,SET_ARPEGGIATOR};
//enum Menu menu = MAIN;

#define MODE_STOP              0
#define MODE_PLAY              1
#define MODE_PAUSE             2
#define MODE_REC               3
#define MODE_REC_TR            4
#define MODE_REC_CUE           5
#define MODE_REC_303           6
#define MODE_ARPEGGIATOR       7
#define MODE_STEP              8
#define MODE_SONG              9
//enum Mode {PLAY,REC,STOP};          // Seqencer: Poly800-Mode, TR-Mode, Cue-Loop, Arpeggiator
//enum Mode mode = PLAY;


// HARDWARE --------------------------------------------------------------------

// device Arduino -> PINs ------------------------------------------------------

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
#define DAC_RESOLUTION         8    // Set this value to 9 (512 values), 8 (256 values), 7 (128 values), 6 (64 values) or 5 (32 values) to adjust the resolution


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


/* HARDWARE ***************************************************************** */

#ifdef DEVICE_MIDI
#endif

#ifdef DEVICE_SYNTH_AUDUINO
#endif

#ifdef DEVICE_ENCODER
#endif

#ifdef DEVICE_SHIFT_REGISTERS
#endif

#ifdef DEVICE_MUX
#endif

#ifdef DEVICE_DAC
#endif

#ifdef DEVICE_NEOPIXELS
#endif

#ifdef DEVICE_DISPLAY
#endif

#ifdef DEVICE_TRELLIS
#endif


/* STATE ******************************************************************** */

struct state_t {
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
};


/* SEQUENCER **************************************************************** */

struct sequencer_t {
    
    byte timecode;                    // time position in pattern
    byte timestep;                    // time in ms per step (1/16)
    byte position;                    // step-number in pattern; 0 <= step < sequencer.song...sequence.length
    byte quantization;                // [1,2,4,8,16]    is 1/1 (whole note) to 1/16th note
    
    struct midi {
        byte clock;
        byte sync;                    // [0,1]        intern , extern
        byte tempo;                   // [1..1000]    bpm
        byte channel;                 // [1..16] || MIDI_CHANNEL_OMNI
        struct channels {
            byte in;
            byte out;
            byte arp;
        } channels;
    } midi;

    struct arpeggiator {
        // notes
        byte chord[NUM_POLYPHON];     // 
        byte *scale;                  //    TODO        scales defined in scales.h
        byte length;                  //                sequence length == num notes in chord
        // parameters
        byte range;                   // [1-5]        octaves
        byte direction;               // [0,1,2,3]    up, down, up and down, random
        byte reset;                   // [n]            reset after n steps
        byte hold;                    // [0,1]        latch
        byte note_length;             // []            staccato..legato
    } arpeggiator;

    struct variation {

        // transpose:                [0..12]            live: use transpositions[] with length 1
        // transpose_octaves:        [n]                use transpositions[] +/- 12 * n
        struct transposition {
            byte active;
            byte sequence[NUM_STEPS];
            byte length;
            byte position;
        } transposition;

        // Umkehrung:
        struct inversion {
            byte type;                // [0,1,2]        0=Grundakkord, 1=Sextakkord, 2=Quartsextakkord
            //byte sequence[NUM_STEPS];
            //byte length;
            //byte position;
        } inversion;
        
        // Krebs:                    [0,1]
        struct reverse {
            byte active;
            //byte sequence[NUM_STEPS];
            //byte length;
            //byte position;
        } reverse;
        
        // Spiegelung an note no. [n]; or mirror at note of sequence | chord position
        struct mirror {
            byte active;
            byte sequence[NUM_STEPS];
            byte length;
            byte position;
        } mirror;
        
        // Arpeggiator octaves
        struct range {
            byte sequence[NUM_STEPS];
            byte directions[NUM_STEPS];
            byte length;
            byte position;
        } range;
        
    } variation;

    struct song {                     // has pattern[8]
        struct pattern {              // 8 song parts; "intro, break1, main, fillAB, variation, fillBA, break2, ending"
            struct track {            // 8 tracks | voices: "lead1, lead2, rhythm, bass, cympals, snare, tomtoms, kick"

                struct sequence {
                    struct step {     // 1 sequence of 32 steps: 2 measures, each with 4 beats, each with 4 steps, each is a 1/16th notes
                        byte note;    // each step with "note | pitch, velocity | accent, slide | legato"
                        byte velocity;
                        byte slide;
                    } step[NUM_STEPS];
                    byte length;      // every track | voice may have a diffenrent length in one pattern
                    byte position;    // use sequence.position instaed!
                } sequence;

                //char* name_of_voice;
                byte channel;         // MIDI-channel or Instrument | Sound
                byte isTransposable;  // QY100
            } track[NUM_TRACKS];      // TODO: mute | solo table

            //char* name_of_part;
            byte active_track;        // record track number n
        } pattern[NUM_PATTERN];

        byte sequence_of_patterns[NUM_SONG_LENGTH];
        byte position;                // position in song == pattern to play
    } song;

};


/* *****************************************************************************
 * Prototypes
 * ************************************************************************** */

// setup(), init...()
// loop(), run(), go()
// get...(), read...()
// set...(), write...()
// update...()
// demo...()

// device Arduino
//void setup(void);
void initHardware(void);
//void loop(void);
void run(void);

void getSerialIn(void);
void getButtonState(void);
void getEncoderChange(void);
void getTrellisButtonState(void);
void getArduinoInputs();

void updateViews(void);
void updateMenu(void);
void updateSequencer(void);

void startSequencer(void);
void stopSequencer(void);
void pauseSequencer(void);
void playSequence(void);
void recordSequence(void);

void sendNoteOn(byte);
void sendCVGate(byte);
void sendSynth(byte);

void blink(long);

// Helpers
float getTimeFromBpm(int);

// TESTS
void testMembers(void);
void test(void);
void testAll(void);
void testUnit(int);

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

#endif // SYNTH_WORKSTATION_H
