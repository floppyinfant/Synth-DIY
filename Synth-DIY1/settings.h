/* Settings for Synth_Workstation */

// DEBUGGING -------------------------------------------------------------------

#define DEBUG                  1    // set 1 for Debug-output to Console
#define TEST                   1
#define PROFILE                1


/* define Modules or comment out */
#define DEVICE_ARDUIUNO_PINS   0
#define DEVICE_EEPROM          1
#define DEVICE_PROGMEM         2
#define DEVICE_SERIAL_IN       3
#define DEVICE_MIDI            4    // MIDI-In, MIDI-Out
#define DEVICE_DAC             5    // CV + Gate
//#define DEVICE_SYNTH_AUDUINO   6
#define DEVICE_PCM             7
#define DEVICE_SHIFT_REGISTERS 8    // (x3)
#define DEVICE_MUX             9    // (x2) -> buttons[], potis[], PIN_ANALOG_IN_...
#define DEVICE_ENCODER        10    // Encoder, RGB_LED
#define DEVICE_NEOPIXELS      11    // 32 pixels
#define DEVICE_DISPLAY        12    // Alphanumeric Display (x2)
#define DEVICE_TRELLIS        13    // Buttons, LEDs

