// ---------------------------------------------------------------------------
//
// midi_cv.ino
//
// A MIDI sequencer example using two MCP4725 dacs for eurorack control voltage
// output and NeoPixels for display.
//
// Required dependencies:
// Adafruit FifteenStep Sequencer Library: https://github.com/adafruit/FifteenStep
// Adafruit NeoPixel Library: https://github.com/adafruit/Adafruit_NeoPixel
// Arduino MIDI library: https://github.com/FortySevenEffects/arduino_midi_library
// arcore: https://github.com/rkistner/arcore
//
// Author: Todd Treece <todd@uniontownlabs.org>
// Copyright: (c) 2015 Adafruit Industries
// License: GNU GPLv3
//
// ---------------------------------------------------------------------------
#include "MIDI.h"
#include "FifteenStep.h"
#include "Adafruit_NeoPixel.h"
#include <SoftwareSerial.h>
#include <Wire.h>

#define NEO_PIN  6
#define LEDS     16
#define TEMPO    90

// sequencer & neopixel init
FifteenStep seq = FifteenStep(1024);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LEDS, NEO_PIN, NEO_GRB + NEO_KHZ800);

// plug midi tx into pin 2
SoftwareSerial midi_serial(2, 3);
MIDI_CREATE_INSTANCE(SoftwareSerial, midi_serial, midi_port);

// start with record mode off
bool record_mode = false;
int steps = 16;

void setup() {
  
 Serial.begin(115200);
  
 // start i2c
 Wire.begin();
 
 // set the midi in callbacks
 midi_port.setHandleNoteOn(handleNoteOn);
 midi_port.setHandleNoteOff(handleNoteOff);
 midi_port.setHandleControlChange(handleControlChange);

 // listen to all channels
 midi_port.begin(MIDI_CHANNEL_OMNI);
 
 // start neopixels
 pixels.begin();
 pixels.setBrightness(80);

 // start sequencer and set callbacks
 seq.begin(TEMPO, steps);
 seq.setMidiHandler(handleSequencerMIDI);
 seq.setStepHandler(handleSequencerStep);
 
}

void loop() {
  
  // check for midi info
  midi_port.read();
 
  // this is needed to keep the sequencer
  // running. there are other methods for
  // start, stop, and pausing the steps
  seq.run();
  
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                          MIDI IN CALLBACKS                                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void handleNoteOn(byte channel, byte pitch, byte velocity) {

  // play pressed note
  handleSequencerMIDI(0, 0x9, pitch, 127);

  // if recording, save note on
  if(record_mode)
    seq.setNote(0, pitch, 127);
    
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {  
  
  // play note off
  handleSequencerMIDI(0, 0x8, pitch, 0x0);

  // if recording, save note off
  if(record_mode)
    seq.setNote(0, pitch, 0x0);
  
}

void handleControlChange(byte channel, byte command, byte value) {
    
  Serial.println(command);
  switch(command) {
    
    case 0:
      record_mode = !record_mode;
      break;
    case 1:
      seq.panic();
      sendVelocity(0);
      break;
    case 7:
      seq.setTempo(map(value, 0, 127, 40, 200));
      break;
    
  }
  
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                        SEQUENCER CALLBACKS                                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// called when the step position changes. both the current
// position and last are passed to the callback
void handleSequencerStep(int current, int last) {
  note_flash(current);
}

// the callback that will be called by the sequencer when 
// it needs to send midi commands.
void handleSequencerMIDI(byte channel, byte command, byte arg1, byte arg2) {

  if(command == 0x9) {
    sendVelocity(4095);
    sendPitch(midiToCV(arg1));
  }
  
  if(command == 0x8) {
    sendVelocity(0);    
  }

}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                          NEOPIXEL HELPERS                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

void note_flash(int current) {

  byte rgb[] = {0,0,0};

  // all LEDs off
  flash(0, 0, 0);

  // make sure we stay within the led count
  current = current % LEDS;

  // highlight quarter notes
  if(current % 4 == 0) {
    // red quarter notes in record mode
    // bright blue in play mode
    if(record_mode)
      rgb[0] = 255;
    else
      rgb[2] = 255;
  } else {
    // dim blue sixteenths
    rgb[2] = 64;
  }

  // highlight note
  show_range(current, current + 1, rgb[0], rgb[1], rgb[2]);

}

// sets all pixels to passed RGB values
void flash(byte r, byte g, byte b) {
  show_range(0, LEDS, r, g, b);
}

// sets range of pixels to RGB values
void show_range(int start, int last, byte r, byte g, byte b) {

  for (; start < last; start++)
    pixels.setPixelColor(start, pixels.Color(r,g,b));

  pixels.show();

}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                            DAC HELPERS                                    //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// send pitch info out to the first dac
void sendPitch(uint16_t output) {
  
  Wire.beginTransmission(0x62);
  Wire.write(0x40);
  Wire.write(output / 16);
  Wire.write((output % 16) << 4);
  Wire.endTransmission();
  
}

// send velocity info out to the first dac
void sendVelocity(uint16_t output) {
  
  Wire.beginTransmission(0x63);
  Wire.write(0x40);
  Wire.write(output / 16);
  Wire.write((output % 16) << 4);
  Wire.endTransmission();
  
}

// convert midi pitch to 12 bit control voltage
int midiToCV(int pitch) {
  return (int) round(((float)pitch - 34.27538387) * 69.400442636);
}