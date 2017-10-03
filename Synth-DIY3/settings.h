/* Settings for Synth_Workstation */

#ifndef SETTINGS_H
#define SETTINGS_H


// DEBUGGING ------------------------------------------------------------------

#define DEBUG                  1    // set 1 for Debug-output to Console
#define TEST                   0
#define PROFILE                1


// MODULES (Hardware) to use --------------------------------------------------
/* define Modules or comment out */

#define DEVICE_ARDUIUNO_PINS   0    // always defined
#define DEVICE_EEPROM          1
#define DEVICE_PROGMEM         2    // @see DEVICE_PCM
#define DEVICE_SERIAL_IN       3
#define DEVICE_MIDI            4    // MIDI-In, MIDI-Out
#define DEVICE_DAC             5    // CV + Gate
//#define DEVICE_SYNTH_AUDUINO   6  // SoftwareSynthesizer
#define DEVICE_PCM             7    // Samples
#define DEVICE_SHIFT_REGISTERS 8    // (x3)
#define DEVICE_MUX             9    // (x2) -> buttons[], potis[], PIN_ANALOG_IN_...
#define DEVICE_ENCODER        10    // Encoder, RGB_LED
#define DEVICE_NEOPIXELS      11    // 32 pixels
#define DEVICE_DISPLAY        12    // Alphanumeric Display (x2)
#define DEVICE_TRELLIS        13    // Buttons, LEDs

/* use this:
#ifdef DEVICE_EEPROM
#endif // DEVICE_EEPROM
#ifdef DEVICE_SYNTH_AUDUINO
#endif // DEVICE_SYNTH_AUDUINO
#ifdef DEVICE_PCM
#endif // DEVICE_PCM
#ifdef DEVICE_SERIAL_IN
#endif // DEVICE_SERIAL_IN
#ifdef DEVICE_MIDI
#endif // DEVICE_MIDI
#ifdef DEVICE_DAC
#endif // DEVICE_DAC
#ifdef DEVICE_SHIFT_REGISTERS
#endif // DEVICE_SHIFT_REGISTERS
#ifdef DEVICE_MUX
#endif // DEVICE_MUX
#ifdef DEVICE_ENCODER
#endif // DEVICE_ENCODER
#ifdef DEVICE_NEOPIXELS
#endif // DEVICE_NEOPIXELS
#ifdef DEVICE_DISPLAY
#endif // DEVICE_DISPLAY
#ifdef DEVICE_TRELLIS
#endif // DEVICE_TRELLIS
*/

#endif // SETTINGS_H
