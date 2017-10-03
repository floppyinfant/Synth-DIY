// Auduino, the Lo-Fi granular synthesiser
//
// by Peter Knight, Tinker.it http://tinker.it
//
// Help:      http://code.google.com/p/tinkerit/wiki/Auduino
// https://code.google.com/archive/p/tinkerit/wikis/Auduino.wiki
//
// Analog in 0: Grain 1 pitch
// Analog in 1: Grain 2 decay
// Analog in 2: Grain 1 decay
// Analog in 3: Grain 2 pitch
// Analog in 4: Grain repetition frequency
//
// Digital 3: Audio out (Digital 11 on ATmega8)
//
// - - -
// Auduino Code put into a class file by tm

#ifndef AUDUINO_H
#define AUDUINO_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

// Map Analogue channels
#define SYNC_CONTROL         (4)
#define GRAIN_FREQ_CONTROL   (0)
#define GRAIN_DECAY_CONTROL  (2)
#define GRAIN2_FREQ_CONTROL  (3)
#define GRAIN2_DECAY_CONTROL (1)

// global variables
uint16_t syncPhaseAcc;
uint16_t syncPhaseInc;
uint16_t grainPhaseAcc;
uint16_t grainPhaseInc;
uint16_t grainAmp;
uint8_t grainDecay;
uint16_t grain2PhaseAcc;
uint16_t grain2PhaseInc;
uint16_t grain2Amp;
uint8_t grain2Decay;

// Changing these will also requires rewriting audioOn()
#if defined(__AVR_ATmega8__)
//
// On old ATmega8 boards.
//    Output is on pin 11
//
#define LED_PIN       13
#define LED_PORT      PORTB
#define LED_BIT       5
#define PWM_PIN       11
#define PWM_VALUE     OCR2
#define PWM_INTERRUPT TIMER2_OVF_vect
#elif defined(__AVR_ATmega1280__)
//
// On the Arduino Mega
//    Output is on pin 3
//
#define LED_PIN       13
#define LED_PORT      PORTB
#define LED_BIT       7
#define PWM_PIN       3
#define PWM_VALUE     OCR3C
#define PWM_INTERRUPT TIMER3_OVF_vect
#else
//
// For modern ATmega168 and ATmega328 boards
//    Output is on pin 3
//
#define PWM_PIN       3
#define PWM_VALUE     OCR2B
#define LED_PIN       13
#define LED_PORT      PORTB
#define LED_BIT       5
#define PWM_INTERRUPT TIMER2_OVF_vect
#endif


class Auduino {

private:

    // Smooth logarithmic mapping
    uint16_t antilogTable[64];

    // Stepped chromatic mapping
    uint16_t midiTable[128];

    // Stepped Pentatonic mapping
    uint16_t pentatonicTable[54];


    uint16_t mapPentatonic(uint16_t input);

    uint16_t mapMidi(uint16_t input);

    uint16_t mapPhaseInc(uint16_t input);


public:
    Auduino(void);
    ~Auduino(void);

    void audioOn(void);
    void update(void);
    
};


SIGNAL(PWM_INTERRUPT)
{
    uint8_t value;
    uint16_t output;

    syncPhaseAcc += syncPhaseInc;
    if (syncPhaseAcc < syncPhaseInc) {
        // Time to start the next grain
        grainPhaseAcc = 0;
        grainAmp = 0x7fff;
        grain2PhaseAcc = 0;
        grain2Amp = 0x7fff;
        LED_PORT ^= 1 << LED_BIT; // Faster than using digitalWrite
    }

    // Increment the phase of the grain oscillators
    grainPhaseAcc += grainPhaseInc;
    grain2PhaseAcc += grain2PhaseInc;

    // Convert phase into a triangle wave
    value = (grainPhaseAcc >> 7) & 0xff;
    if (grainPhaseAcc & 0x8000) value = ~value;
    // Multiply by current grain amplitude to get sample
    output = value * (grainAmp >> 8);

    // Repeat for second grain
    value = (grain2PhaseAcc >> 7) & 0xff;
    if (grain2PhaseAcc & 0x8000) value = ~value;
    output += value * (grain2Amp >> 8);

    // Make the grain amplitudes decay by a factor every sample (exponential decay)
    grainAmp -= (grainAmp >> 8) * grainDecay;
    grain2Amp -= (grain2Amp >> 8) * grain2Decay;

    // Scale output to the available range, clipping if necessary
    output >>= 9;
    if (output > 255) output = 255;

    // Output to PWM (this is faster than using analogWrite)  
    PWM_VALUE = output;
}

#endif // AUDUINO_H
