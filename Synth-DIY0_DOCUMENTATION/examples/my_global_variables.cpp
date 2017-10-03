/** Global Variables */
// DAC
Adafruit_MCP4725 dac;
// Auduino
// Shift-Registers
#define number_of_74hc595s 3                        // change this: how many of the shift registers
#define NUM_REGISTER_PINS number_of_74hc595s * 8    // do not touch
boolean registers[NUM_REGISTER_PINS];
// MUX
// Rotary Encoder
#define ENC_PORT PINC
// Neopixels
Adafruit_NeoPixel strip;
// alphanum Display
Adafruit_AlphaNum4 alpha4;
// Trellis
#define MOMENTARY 0
#define LATCHING 1
#define MODE LATCHING // set the mode here
Adafruit_Trellis matrix0;
Adafruit_TrellisSet trellis;
// Sequencer
#define STEPS 128           // array length = 4 steps * 4 beats * 8 Measures (=32 beats max) =
#define NUM_POTIS 8
#define NUM_BUTTONS 16

byte potis[NUM_POTIS];
byte buttons[NUM_BUTTONS];

byte time;                  // profiling
byte loops;                 // profiling

long clock;
long nextStep;
// save these to EEPROM
byte sequence[STEPS];
byte sequence_slides[STEPS];        // slide | note length (staccato, legato -> portamento)
byte sequence_velocities[STEPS];    // accent | velocity: normal is 100, accent is 127
byte sequence_ranges[STEPS];
byte sequence_directions[STEPS];
byte sequence_length;               // steps: beats * 4; must be <= STEPS (<= 128 steps bzw. 32 beats)

byte transpositions[STEPS];
byte transpositions_length;

byte var_transpose;         // {x*(-12) | 0 - 12 | x*(+12)} midi note numbers
byte var_inversion;         // Umkehrung {0,1,2}, 1: Sextakkord, 2: Quartsextakkord, 0: Grundakkord
byte var_mirror_at;         // Spiegelung an note number ...
byte var_reversing;         // Krebs {0,1}

byte arp_length;            // note length
byte arp_scale;             // index of array defined in scales.h
byte arp_hold;              // on | off
byte arp_octaves;           // 1 - 5
byte arp_direction;         // up, down, updown, random
byte arp_rate;              // same as quantize
byte arp_reset;             // reset after X steps of sequence: {0 = sequence.lenth, 1, ..., 127}, if (reset > sequence.length) do (fill by looping the sequence)
// Preferences, Settings, Parameters
byte midiChannel = MIDI_CHANNEL_OMNI; // {1..16 | MIDI_CHANNEL_OMNI}
byte sync_to_midi_clk;
byte tempo;                 // bpm; 0 < clk
byte onestep;
byte quantization;          // {1/16, 1/8, 1/4, 1/2, 1/1}

enum Mode {PLAY,REC,STOP};  // Seqencer: Poly800-Mode, TR-Mode, Cue-Loop, Arpeggiator
enum Mode mode = PLAY;

enum Menu {MAIN,SET_CLOCK,SET_SEQUENCE,SET_TRANSPOSITION,SET_VARIATION,SET_ARPEGGIATOR};
enum Menu menu = MAIN;
