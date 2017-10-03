#include <Arduino.h>

/***************************************************************
 ** Groovesizer - 16-step sequencer and synth
 **
 ** by Moshang - http://moshang.net
 **
 ** more info at http://tinyurl.com/groovesizer
 **
 ** based on Auduino, the Lo-Fi granular synthesiser - info below
 **
 ***************************************************************/

// Auduino, the Lo-Fi granular synthesiser
//
// by Peter Knight, Tinker.it http://tinker.it
//
// Help:      http://code.google.com/p/tinkerit/wiki/Auduino
// More help: http://groups.google.com/group/auduino



#include <EEPROM.h> //for reading and writing patterns to permanent memory

#include <avr/io.h>
#include <avr/interrupt.h>

//updated with volatile fix
uint16_t syncPhaseAcc;
volatile uint16_t syncPhaseInc;
uint16_t grainPhaseAcc;
volatile uint16_t grainPhaseInc;
uint16_t grainAmp;
volatile uint8_t grainDecay;
uint16_t grain2PhaseAcc;
volatile uint16_t grain2PhaseInc;
uint16_t grain2Amp;
volatile uint8_t grain2Decay;


// Map Analogue channels
#define CONTROL         (4)
#define GRAIN_FREQ_CONTROL   (0)
#define GRAIN_DECAY_CONTROL  (2)
#define GRAIN2_FREQ_CONTROL  (3)
#define GRAIN2_DECAY_CONTROL (1)


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
//#define LED_PIN       13
//#define LED_PORT      PORTB
//#define LED_BIT       5
#define PWM_INTERRUPT TIMER2_OVF_vect
#endif


// Smooth logarithmic mapping
//
uint16_t antilogTable[] = {
  64830,64132,63441,62757,62081,61413,60751,60097,59449,58809,58176,57549,56929,56316,55709,55109,
  54515,53928,53347,52773,52204,51642,51085,50535,49991,49452,48920,48393,47871,47356,46846,46341,
  45842,45348,44859,44376,43898,43425,42958,42495,42037,41584,41136,40693,40255,39821,39392,38968,
  38548,38133,37722,37316,36914,36516,36123,35734,35349,34968,34591,34219,33850,33486,33125,32768
};
uint16_t mapPhaseInc(uint16_t input) {
  return (antilogTable[input & 0x3f]) >> (input >> 6);
}


// Stepped chromatic mapping

uint16_t midiTable[] = {
  0,
  69,73,77,82,86,92,97,103,109, 115,122,129,
  137,145,154,163,173,183,194,206,218,231,244,259,
  274,291,308,326,346,366,388,411,435,461,489,518,
  549,581,616,652,691,732,776,822,871,923,978,1036,
  1097,1163,1232,1305,1383,1465,1552,1644,1742,1845,1955,2071,
  2195,2325,2463,2610,2765,2930,3104,3288,3484,3691,3910,4143,
  4389,
  9999
};

uint8_t pitchIndexNum(uint16_t input) {
  input = (input > 1015) ? 1015 : input;
  int i = (1015-input) / (1015/74);
  int j = (i < 74) ? i : 74; 
  return j;
}

byte pitchIndex[16] = {
  37,0,0,0,37,0,0,0,37,0,0,0,37,0,0,0
};

// shave off the upper and lower extremes to allow for (my) dodgy potentiometers
int minPot0 = 25;
int minPot1 = 25;
int minPot2 = 25;
int minPot3 = 25;
int minPot4 = 25;

int maxPot0 = 1000;
int maxPot1 = 1000;
int maxPot2 = 1000;
int maxPot3 = 1000;
int maxPot4 = 1000;

int nowPot3;
int nowPot2;
int nowPot1;
int nowPot0;
int nowPot4;

byte stepNum = 0;
byte blink1Led = 0;
byte seqLength = 15;
boolean seqLengthPU = false;
int stepDur = 150;
int swingStep = 150;
boolean stepDurPU = false; //PU = pickup
int stepRest = 40;
boolean stepRestPU = false;
int noteDur = stepDur - stepRest;
int swing = 0; //amount of swing in milliseconds
boolean swingPU = false;
int swingPot = 1000; //keep track of the actual pot value for pickup

int Gr2Freq = mapPhaseInc(nowPot0);
int GrDec = nowPot1 / 8;
int Gr2Dec = nowPot2 / 4;
int GrFreq = mapPhaseInc(nowPot3) / 2;

boolean Gr2FreqPU = true;
boolean GrDecPU = true;
boolean Gr2DecPU = true;
boolean GrFreqPU = true;

boolean stepPU[16] = {
  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

boolean reverse = false;

boolean patMode = false; // is pattern mode enabled or not
boolean lastMode = false; // keep track of the last mode so LEDs are only turned off once

byte memory = 0;

boolean memoryPU = false;
boolean armProm[4] = {
  false, false, false, false};
boolean armSave[4] = {
  false, false, false, false};
unsigned long longPress = 0;
boolean quantize = true;
byte triggerButton;
boolean changedPat = true; //to make sure the pattern only gets changed once
unsigned long saveBlink = 0;
boolean armedBlink = false;
boolean blinkOn = true;

unsigned long storedBlink = 0; // blinkpattern on successful save
boolean blinkStoredOn = false;

boolean noteOffSwitch = false;
unsigned long noteOn = millis();
unsigned long noteOff = noteOn + noteDur;
unsigned long stepEnd = noteOn + stepDur;
unsigned long stepCount = 0;
unsigned long functionDebounce = 0;
unsigned long tempoTap1 = 0;
unsigned long tempoTap2 = 0;

int tempos[10];
byte tapNum = 0;
byte temposCheck = 0;

boolean tap1 = true;

boolean tie = false;

boolean played = false; // has the midi note been played yet

boolean calibrate = false;

/* MIDI-out based on:
 MIDI On Messages with 0 velocity
 By Amanda Ghassaei
 July 2012
 http://www.instructables.com/id/Send-and-Receive-MIDI-with-Arduino/
 */

int velocity = 100;//velocity of MIDI notes, must be between 0 and 127
//(higher velocity usually makes MIDI instruments louder)

int noteON = 144;//144 = 10010000 in binary, note on command

int MIDInote = 60;


//Adafruit Button Checker http://www.adafruit.com/blog/2009/10/20/example-code-for-multi-button-checker-with-debouncing/
#define DEBOUNCE 10  // button debouncer, how many ms to debounce, 5+ ms is usually plenty

// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = {
  0, 2, 4, 5, 19}; // the analog 0-5 pins are also known as 14-19 ---- {2, 4, 5, 14}
// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)
// we will track if a button is just pressed, just released, or 'currently pressed' 
byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];

void audioOn() {
#if defined(__AVR_ATmega8__)
  // ATmega8 has different registers
  TCCR2 = _BV(WGM20) | _BV(COM21) | _BV(CS20);
  TIMSK = _BV(TOIE2);
#elif defined(__AVR_ATmega1280__)
  TCCR3A = _BV(COM3C1) | _BV(WGM30);
  TCCR3B = _BV(CS30);
  TIMSK3 = _BV(TOIE3);
#else
  // Set up PWM to 31.25kHz, phase accurate
  TCCR2A = _BV(COM2B1) | _BV(WGM20);
  TCCR2B = _BV(CS20);
  TIMSK2 = _BV(TOIE2);
#endif
}

void setup() {

  // Set MIDI baud rate:
  Serial.begin(31250);

  //Serial.begin(9600); // for debugging

  pinMode(PWM_PIN,OUTPUT);
  audioOn();

  for (int MSledPin = 6; MSledPin < 14; MSledPin++)
  {
    pinMode(MSledPin,OUTPUT);
  }

  //Adafruit Button Checker
  byte i;

  // Make input & enable pull-up resistors on switch pins
  for (i=0; i< NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }
}


void loop() {

  //Adafruit Button Checker
  check_switches();      // when we check the switches we'll get the current state

  // Get current pot values
  nowPot0 = constrain(analogRead(GRAIN2_FREQ_CONTROL), minPot0, maxPot0);
  nowPot0 = map(nowPot0, minPot0, maxPot0, 0, 1023);


  nowPot1 = constrain(analogRead(GRAIN_DECAY_CONTROL), minPot1, maxPot1);
  nowPot1 = map(nowPot1, minPot1, maxPot1, 0, 1023);

  nowPot2 = constrain(analogRead(GRAIN2_DECAY_CONTROL), minPot2, maxPot2);
  nowPot2 = map(nowPot2, minPot2, maxPot2, 0, 1023);

  nowPot3 = constrain(analogRead(GRAIN_FREQ_CONTROL), minPot3, maxPot3);
  nowPot3 = map(nowPot3, minPot3, maxPot3, 0, 1023);

  nowPot4 = constrain(analogRead(CONTROL), minPot4, maxPot4);
  nowPot4 = map(nowPot4, minPot4, maxPot4, 0, 1023);

  if (pressed[0] && justpressed[4]) // toggle pattern mode on/off
  {
    unsigned long current = millis();
    if (current - functionDebounce > 300)
    {
      patMode = !patMode;
      for (byte i = 0; i < 4; i++)
      {
        armProm[i] = false;
        armSave[i] = false;
      }
      functionDebounce = current;
      if (!patMode && lastMode == true)
      {
        for (byte i = 6; i < 14; i++)
          digitalWrite (i, LOW); // turn of all the LEDs

        longPress = 0;
        memoryPU = false;
      }
      else
        longPress = millis();

      lastMode = patMode;
    }
  }

  if (patMode)
  {
    if (!armedBlink && !blinkStoredOn)
    { 
      for (byte i = 6; i < 14; i++)
        digitalWrite (i, LOW); // turn off all the LEDs

      // turn on the LEDs for the memory location
      int v = 0; // a variable name other than i
      do
      {
        int j = v + 6; // LEDs start at pin 6
        digitalWrite (j, HIGH);
        v++;
      }
      while (v < (memory + 1));
    }
    else if (armedBlink && !blinkStoredOn)
      saveArmBlink();

    else 
      saveDone();  

    //select the memory location
    memory = (memoryPU) ? map(nowPot4, 0, 1023, 7, 0) : memory;
    memoryPU = (map(nowPot4, 0, 1023, 7, 0) == memory) ? true : false;

    // check button presses in pat mode
    for (int k = 0; k < 4; k++)
    {
      if (justpressed[k])
        patJustPressed(k);
    }

    for (int k = 0; k < 4; k++)
    {
      if (justreleased[k])
        patJustReleased(k);
    }


    for (int k = 0; k < 4; k++)
    {
      if (!armSave[k])
      {
        if (pressed[k])
          patPressed(k);
      }
    }
  }

  if (justpressed[3] && pressed[4]) //retrigger the pattern from the first step
  {
    unsigned long current = millis();
    if (current - functionDebounce > 300)
    {

      functionDebounce = current;

      stepNum = 0;
      stepCount = 0;

      noteOn = millis();
      noteOff = noteOn + noteDur;
      swingStep = (stepCount % 2 == 0) ? stepDur + swing : stepDur - swing;
      stepEnd = noteOn + swingStep;
      noteOffSwitch = false;

      //tap tempo with running average

        if (tap1)
      {
        tempoTap1 = noteOn; // current time
        tempoTap2 = ((tempoTap1 - tempoTap2) > 1200) ? 0 : tempoTap2;
        int j = 0;
        if (tempoTap2 != 0)
        {
          tempos[tapNum] = ((tempoTap1 - tempoTap2) / 4);
          for (int i = 0; i < 10; i++)
            j = j + tempos[i];
          temposCheck = (temposCheck < 10) ? temposCheck + 1 : 10;
          stepDur = j / (temposCheck);
          tapNum = (tapNum < 9) ? tapNum + 1 : 0;  
        }
        else 
        {
          stepDur = stepDur;
          for (int i = 0; i < 10; i++)
            tempos[i] = 0;
          tapNum = 0;
          temposCheck = 0; 
        }
        //stepDur = (tempoTap2 != 0) ? ((tempoTap1 - tempoTap2) / 4) : stepDur;   
      }

      if (!tap1)
      {
        tempoTap2 = noteOn; // current time
        tempoTap1 = ((tempoTap2 - tempoTap1) > 1200) ? 0 : tempoTap1;
        int j = 0;
        if (tempoTap1 != 0)
        {
          tempos[tapNum] = ((tempoTap2 - tempoTap1) / 4);
          for (int i = 0; i < 10; i++)
            j = j + tempos[i];
          temposCheck = (temposCheck < 10) ? temposCheck + 1 : 10;
          stepDur = j / (temposCheck);
          tapNum = (tapNum < 9) ? tapNum + 1 : 0;  
        }
        else 
        {
          stepDur = stepDur;
          for (int i = 0; i < 10; i++)
            tempos[i] = 0;
          tapNum = 0;
          temposCheck = 0;  
        }
        //stepDur = (tempoTap1 != 0) ? ((tempoTap2 - tempoTap1) / 4) : stepDur;
      }

      tap1 = !tap1;

      //turn off all LEDs 
      for (int i = 6; i < 14; i++)
        digitalWrite (i, LOW);
    }
  }

  else if (justpressed[2] && pressed[4]) // reverse switch
  {
    unsigned long current = millis();
    if (current - functionDebounce > 300)
    {
      reverse = !reverse;
      functionDebounce = current;
    }
  }

  else if (justpressed[1] && pressed[4]) // shift the pattern up a semitone
  {
    unsigned long current = millis();
    if (current - functionDebounce > 300)
    {
      for (int i = 0; i < 16; i++)
      {
        if (pitchIndex[i] == 0)
          pitchIndex[i] = 0;
        else if (pitchIndex[i] == 74)
          pitchIndex[i] = 74;  
        else
        {
          pitchIndex[i] = pitchIndex[i] + 1;
          pitchIndex[i] = (pitchIndex[i] > 73) ? 1 : pitchIndex[i];
        }
      }
      functionDebounce = current;
    }
  }

  else if (justpressed[0] && pressed[4]) // shift the pattern down a semitone
  {
    unsigned long current = millis();
    if (current - functionDebounce > 300)
    {
      for (int i = 0; i < 16; i++)
      {
        if (pitchIndex[i] == 0)
          pitchIndex[i] = 0;
        else if (pitchIndex[i] == 74)
          pitchIndex[i] = 74;  
        else
        {
          pitchIndex[i] = pitchIndex[i] - 1;
          pitchIndex[i] = (pitchIndex[i] < 1) ? 73 : pitchIndex[i];
        }
      }
      functionDebounce = current;
    }
  } 

  else if (justpressed[0] || justpressed[1] || justpressed[2] || justpressed[3] || justpressed[4])
  {
    Gr2Freq = (Gr2FreqPU) ? mapPhaseInc(nowPot0) : Gr2Freq;
    GrDec = (GrDecPU) ? nowPot1 / 8 : GrDec;
    Gr2Dec = (Gr2DecPU) ? nowPot2 / 4 : Gr2Dec;
    GrFreq = (GrFreqPU) ? mapPhaseInc(nowPot3) / 2 : GrFreq;

    for (int i = 0; i < 16; i++)
      stepPU[i] = false;

    stepDurPU = false;
    stepRestPU = false;
    seqLengthPU = false;
    swingPU = false;
  }  

  else if (pressed[0] || pressed[1] || pressed[2] || pressed[3] || pressed[4])
  {
    if (pressed[0] && !pressed[4] && !patMode)
    { 
      //turn on LED 1 
      digitalWrite (6, HIGH);
      for (int i = 0; i < 4; i++)  
        updatePitch(i);   
    }

    if (pressed[1] && !pressed[4] && !patMode)
    {
      digitalWrite (10, HIGH);
      for (int i = 4; i < 8; i++)  
        updatePitch(i); 
    }

    if (pressed[2] && !pressed[4] && !patMode)
    {
      //turn on LEDs 1 & 2
      digitalWrite (6, HIGH);
      digitalWrite (7, HIGH);
      for (int i = 8; i < 12; i++)  
        updatePitch(i);
    }

    if (pressed[3] && !pressed[4] && !patMode)
    {
      //turn on LEDs 5 & 6  
      digitalWrite (10, HIGH);
      digitalWrite (11, HIGH);  
      for (int i = 12; i < 16; i++)  
        updatePitch(i);
    }

    if (pressed[4] && !patMode)
    {
      //CONTROL

      int currentCONTROL = map(nowPot4, 0, 1023, 50, 300);
      stepDur = (stepDurPU) ? currentCONTROL : stepDur;
      stepDurPU = (currentCONTROL == stepDur) ? true : false; 

      stepRest = (stepRestPU) ? map(nowPot3, 0, 1023, 0, (stepDur-20)) : stepRest;
      stepRestPU = (map(nowPot3, 0, 1023, 0, (stepDur-20)) == stepRest) ? true : false;
      noteDur = stepDur - stepRest;
      swing = (swingPU) ? map(nowPot2, 0, 1023, ((stepDur*2)/3)*2-stepDur, 0) : swing;
      swingPot = (swingPU) ? nowPot2 : swingPot;
      swingPU = (nowPot2 > swingPot - 10 && nowPot2 < swingPot + 10) ? true : false;
      seqLengthPU = false;

      Gr2FreqPU = false;
      GrDecPU = false;
      Gr2DecPU = false;
      GrFreqPU = false;
    }

  }

  else if (justreleased[0] || justreleased[1] || justreleased[2] || justreleased[3] || justreleased[4])
  {
    stepDurPU = false;

    for (int i = 0; i < 16; i++)
      stepPU[i] = false;

    if (justreleased[0]) 
    {
      //turn off LED 1 
      digitalWrite (6, LOW); 
    }

    if (justreleased[1]) 
    {
      //turn off LED 5 
      digitalWrite (10, LOW); 
    }

    if (justreleased[2]) 
    {
      //turn off LED 1 & 2 
      digitalWrite (6, LOW); 
      digitalWrite (7, LOW);
    }

    if (justreleased[3]) 
    {
      //turn off LED 5 & 6 
      digitalWrite (10, LOW); 
      digitalWrite (11, LOW);
    }
  }

  else 
  {
    if (!patMode)
    {
      if (seqLengthPU)
      {
        seqLength = map(nowPot4, 0, 1023, 15, 0); 
      }
    }

    if (Gr2FreqPU)
      grain2PhaseInc = mapPhaseInc(nowPot0) / 2;

    if (GrDecPU)
      grainDecay = nowPot1 / 8;

    if (Gr2DecPU)
      grain2Decay = nowPot2 / 4;

    if (GrFreqPU)
      grainPhaseInc  = mapPhaseInc(nowPot3) / 2;

    if (!patMode)
    {
      if (!seqLengthPU)
      {
        //nowPot4 = (nowPot4 < 200) ? 200 : nowPot4;
        seqLengthPU = (map(nowPot4, 0, 1023, 15, 0) == seqLength) ? true : false;
      }
    }

    if (!Gr2FreqPU)
      Gr2FreqPU = (mapPhaseInc(nowPot0) / 2 == Gr2Freq) ? true : false;

    if (!GrDecPU)
      GrDecPU = (nowPot1 / 8 == GrDec) ? true : false;

    if (!Gr2DecPU)
      Gr2DecPU = (nowPot2 / 4 == Gr2Dec) ? true : false;

    if (!GrFreqPU)
      GrFreqPU = (mapPhaseInc(nowPot3) / 2 == GrFreq) ? true : false;    
  } 

  //play the note for this step if the note isn't off and deal with ties

  int x = nextStep(stepNum);
  tie = (pitchIndex[x] == 74) ? true : false;

  if (pitchIndex[stepNum] != 74)
  { 
    syncPhaseInc = (!noteOffSwitch) ? (midiTable[pitchIndex[stepNum]]) : 0; //play a note or turn it off

    if (!played)
    {
      //MIDI note on
      MIDInote = (pitchIndex[stepNum] > 0) ? (pitchIndex[stepNum] + 23) : MIDInote;

      if (pitchIndex[stepNum] == 0)
        MIDImessage(noteON, MIDInote, 0);//turn midi note off
      else 
        MIDImessage(noteON, MIDInote, velocity);//turn note on
      played = true;
    }
  }



  if (!patMode)
  {
    blink1Led = (stepNum < 8) ? stepNum + 6 : (stepNum%8) + 6;
    digitalWrite (blink1Led, HIGH);

    if (stepNum > 7)
      digitalWrite (13, HIGH);
    if (stepNum < 8 && stepNum != 7 && stepNum != 15)
      digitalWrite (13, LOW);
  }

  else
  {
    if (stepNum == 0)
    {
      for (byte i = 6; i < 14; i++)
        digitalWrite (i, HIGH); // turn on all the LEDs       
    }
    else
    {
      for (byte i = 6; i < 14; i++)
        digitalWrite (i, LOW); // turn on all the LEDs 
    }
  }

  if (millis() >= noteOff)
  {
    digitalWrite (blink1Led, LOW);
    if (!tie)
    {
      noteOffSwitch = true;
      //turn off the playing note
      syncPhaseInc = 0;
      //MIDI note off
      MIDImessage(noteON, MIDInote, 0);
    }
  }

  if (millis() >= stepEnd)
  {
    digitalWrite (blink1Led, LOW);
    if (!reverse)
    {
      stepNum = (stepNum >= seqLength) ? 0 : stepNum + 1;
      if (stepNum == 0 && quantize && !changedPat)
      {
        loadPat(triggerButton);
        changedPat = true;
      }
      stepCount++;
    }
    else
    {
      stepNum = (stepNum == 0) ? seqLength : stepNum - 1;
      if (stepNum == seqLength && quantize && !changedPat)
      {
        loadPat(triggerButton);
        changedPat = true;
      }
      stepCount--;
    }  
    noteOn = millis();
    noteOff = (stepCount % 2 == 0) ? noteOn + noteDur + swing : noteOn + noteDur - (swing/4);
    swingStep = (stepCount % 2 == 0) ? stepDur + swing : stepDur - swing;
    stepEnd = noteOn + swingStep;
    noteOffSwitch = false;
    played = false; // the step has ended, so the next midi note can be played 

  }

}

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
    //LED_PORT ^= 1 << LED_BIT; // Faster than using digitalWrite
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

// send MIDI message
void MIDImessage(int command, int MIDI_note, int MIDIvelocity) {
  Serial.write(command);//send note on or note off command 
  Serial.write(MIDI_note);//send pitch data
  Serial.write(MIDIvelocity);//send velocity data
}

// assign notes to steps
void updatePitch (int thisStep) {
  int thisStepPitch;
  if (thisStep == 0 || thisStep == 4 || thisStep == 8 || thisStep == 12)
  {
    thisStepPitch = pitchIndexNum(nowPot0);
    Gr2FreqPU = false; 
  }
  else if (thisStep == 1 || thisStep == 5 || thisStep == 9 || thisStep == 13)
  {
    thisStepPitch = pitchIndexNum(nowPot1);
    GrDecPU = false;  
  }
  else if (thisStep == 2 || thisStep == 6 || thisStep == 10 || thisStep == 14)
  {
    thisStepPitch = pitchIndexNum(nowPot2);
    Gr2DecPU = false;  
  }
  else if (thisStep == 3 || thisStep == 7 || thisStep == 11 || thisStep == 15)
  {
    thisStepPitch = pitchIndexNum(nowPot3);
    GrFreqPU = false;  
  }
  pitchIndex[thisStep] = (stepPU[thisStep]) ? thisStepPitch :pitchIndex[thisStep];
  stepPU[thisStep] = (thisStepPitch == pitchIndex[thisStep]) ? true : false;
}

void patJustPressed(byte x)
{
  armedBlink = false;

  if (!armProm[x] && !armSave[x])
  {
    // first clear all arms
    for (int i = 0; i < 4; i++)
    {
      armProm[i] = false;
      armSave[i] = false;
    }
    longPress = millis();
    armProm[x] = true;


  }
  else if (armSave[x])
  {

    int z;
    int y = (x * 16) + (memory * 64);
    for (byte i = 0; i < 16; i++)
    {
      z = y + i;
      EEPROM.write(z, pitchIndex[i]);
    }
    for (byte i = 0; i < 4; i++)
    {
      armProm[i] = false;
      armSave[i] = false;
    }
    longPress = 0;
    saveBlink = 0;
    blinkStoredOn = true;
    storedBlink = millis();
  }

}

void patJustReleased(byte x)
{
  if (armProm[x])
  {
    if (!armSave[x])
    {
      //change the pattern, son
      if (pressed[4])
      {
        loadPat(x);
        quantize = false;  
      }
      else
      {
        triggerButton = x;
        quantize = true;
        changedPat = false;
      }
      //tmp check
      for (byte i = 6; i < 14; i++)
        digitalWrite (i, HIGH); // turn on all the LEDs

      longPress = 0;
      for (byte i = 0; i < 4; i++)
      {
        armProm[i] = false;
        armSave[i] = false;
      }
    }

  } 
}

void patPressed(byte x)
{
  if ((millis() - longPress) >= 1000 && longPress != 0)
  {
    armSave[x] = true;
    longPress = 0;
    armedBlink = true;
    saveBlink = millis();
  }
}

void loadPat(byte x)
{
  //change the pattern, son
  int z;
  int y = (x * 16) + (memory * 64); 
  for (byte i = 0; i < 16; i++)
  {
    z = y + i; 
    pitchIndex[i] = EEPROM.read(z);
  }
}

byte nextStep(byte x)
{
  if (!reverse)
  {
    byte y = ((x + 1) < 16) ? (x + 1) : 0;
    return y;
  }
  else
  {
    byte y = (x > 0) ? (x - 1) : 15;
    return y;
  }
}

void saveArmBlink()
{
  if ((millis() - saveBlink) > 150 && saveBlink != 0)
  {
    blinkOn = !blinkOn;
    saveBlink = millis();
  }  
  if (blinkOn)
  {
    for (byte i = 6; i < 14; i++)
      digitalWrite (i, HIGH); // turn on all the LEDs
  }
  else if (!blinkOn)
  {
    for (byte i = 6; i < 14; i++)
      digitalWrite (i, LOW); // turn off all the LEDs 
  } 
}

void saveDone()
{
  unsigned long nowTime = millis();

  if ((nowTime - storedBlink) < 100)
  {
    digitalWrite (9, HIGH);
    digitalWrite (10, HIGH);
  }
  if ((nowTime - storedBlink) >= 200 && (nowTime - storedBlink) < 300)
  {

    digitalWrite (8, HIGH);
    digitalWrite (11, HIGH);
  }
  if ((nowTime - storedBlink) >= 300 && (nowTime - storedBlink) < 400)
  {
    digitalWrite (9, LOW);
    digitalWrite (10, LOW);
    digitalWrite (7, HIGH);
    digitalWrite (12, HIGH);
  }
  if ((nowTime - storedBlink) >= 300 && (nowTime - storedBlink) < 400)
  {
    digitalWrite (8, LOW);
    digitalWrite (11, LOW);
    digitalWrite (6, HIGH);
    digitalWrite (13, HIGH);
  }
  if ((nowTime - storedBlink) > 500)
  {
    blinkStoredOn = false;
    storedBlink = 0;
  }
}

//Adafruit Button Checker
void check_switches()
{
  static byte previousstate[NUMBUTTONS];
  static byte currentstate[NUMBUTTONS];
  static long lasttime;
  byte index;

  if (millis() < lasttime) {
    // we wrapped around, lets just try again
    lasttime = millis();
  }

  if ((lasttime + DEBOUNCE) > millis()) {
    // not enough time has passed to debounce
    return; 
  }
  // ok we have waited DEBOUNCE milliseconds, lets reset the timer
  lasttime = millis();

  for (index = 0; index < NUMBUTTONS; index++) {
    justpressed[index] = 0;       // when we start, we clear out the "just" indicators
    justreleased[index] = 0;

    currentstate[index] = digitalRead(buttons[index]);   // read the button

    if (currentstate[index] == previousstate[index]) {
      if ((pressed[index] == LOW) && (currentstate[index] == LOW)) {
        // just pressed
        justpressed[index] = 1;
      }
      else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH)) {
        // just released
        justreleased[index] = 1;
      }
      pressed[index] = !currentstate[index];  // remember, digital HIGH means NOT pressed
    }
    //Serial.println(pressed[index], DEC);
    previousstate[index] = currentstate[index];   // keep a running tally of the buttons
  }
}
