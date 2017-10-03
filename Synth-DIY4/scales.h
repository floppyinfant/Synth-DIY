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
 * @date 2015-06-01, v0.9
 * @version 0.9
 * @license Public Domain | CC | (L)GPL | Apache | BSD | MIT | Proprietary
 * https://tldrlegal.com/
 * Open Source, Open Hardware
 *
 * Copyright by Thorsten Mauthe
 * floppyinfant.com - all right reserved
 *
 *
 * TODO:
 * PROGMEM
 *
 *
 *****************************************************************************/

#ifndef SCALES_H
#define SCALES_H

// SCALES/ TONLEITERN ----------------------------------------------------------

// Dur, major, ionian
const byte SCALE_MAJOR[] = {0,2,4,5,7,9,11};

// moll, minor, aeolian
const byte SCALE_MINOR[] = {0,2,3,5,7,8,10};

const byte SCALE_HARMONIC_MINOR[] = {0,2,3,5,7,8,11};

// Jazz
const byte SCALE_MELODIC_MINOR[] = {0,2,3,5,7,9,11};

// Rock, Blues
const byte SCALE_MINOR_PENTATONIC[] = {0,3,5,7,10};

// Country
const byte SCALE_MAJOR_PENTATONIC[] = {0,2,4,7,9};

// Jazz, Rock, Blues; minor pentatonic + b5
const byte SCALE_BLUES[] = {0,3,5,6,7,10};

// Jazz, Heavy Metal
const byte SCALE_DIMINISHED[] = {0,2,3,5,6,8,9,11};

// Zwoelftonmusik --------------------------------------------------------------
// cis, gis, e, dis, his (=c), fis, ais, h, a, d, f, g
const byte SCALE_TWELVETONE[12] = {1,8,4,3,0,6,10,11,9,2,5,7};

// CHORDS/ AKKORDE -------------------------------------------------------------

// DUR
const byte SCALE_MAJOR_CHORD[] = {0,4,7};

// moll
const byte SCALE_MINOR_CHORD[] = {0,3,7};

// Dominantsept-Akkord, C7
const byte SCALE_SEPT_CHORD[] = {0,4,7,10};

// DUR-Dreiklang mit großer Sexte, C6
const byte SCALE_SEXT_CHORD[] = {0,4,7,9};

// DUR mit grosser None
const byte SCALE_NONE_CHORD[] = {0,4,7,14};

// verminderter Akkord, C0, Cdim, Cdim7
const byte SCALE_DIM_CHORD[] = {0,3,6,9};

// DUR mit übermaessiger Qubytee, C+5, C5+, Caug
const byte SCALE_AUG_CHORD[] = {0,4,8};

// DUR mit verminderter Qubytee, C-5, C5-
const byte SCALE_FIVEMINUS_CHORD[] = {0,4,6};

// DUR mit Quart-Vorhalt, C4, Csus, Csus4
const byte SCALE_SUS_CHORD[] = {0,5,7};

// Septakkord mit Quartvorhalt, C7/4, C7sus4
const byte SCALE_SEPTQUART_CHORD[] = {0,5,7,10};

// Septakkord mit ueberlegter Sexte, C7/6
const byte SCALE_SEPTSEXT_CHORD[] = {0,4,7,9,10};

//DUR mit kleiner Septime und grosser None, C7/9
const byte SCALE_SEPTNONE_CHORD[] = {0,4,7,10,14};

// DUR mit grosser Sexte und grosser None, C6/9
const byte SCALE_SEXTNONE_CHORD[] = {0,4,7,9,14};

// DUR mit grosser Septime, Cmaj7, CM7
const byte SCALE_MAJORSEPT_CHORD[] = {0,4,7,11};

// DUR mit grosser Septime und grosser None, Cmaj7/9, Cj7/9, CM7/9
const byte SCALE_MAJORSEPTNONE_CHORD[] = {0,4,7,11,14};

// moll mit grosser Sexte, Cm6
const byte SCALE_MSEXT_CHORD[] = {0,3,7,9};

// moll mit kleiner Septime, Cm7, Cmin7
const byte SCALE_MSEPT_CHORD[] = {0,3,7,10};

// moll mit kleiner Septime und grosser None, Cm7/9, Cm9, Cmin7/9
const byte SCALE_MSEPTNONE_CHORD[] = {0,3,7,10,14};

// moll mit kleiner Septime und verminderter Qubytee, Cm7-5, Cm7/5(b), Cm7b5
const byte SCALE_MSEPTFIVEMINUS_CHORD[] = {0,3,6,10};

// Septakkord mit uebermaessiger Qubytee, C7+5, C+7
const byte SCALE_SEPTAUG_CHORD[] = {0,4,8,10};

// Septakkord mit verminderter Qubytee, C7-5, C7/5b
const byte SCALE_SEPTFIVEMINUS_CHORD[] = {0,4,6,10};

// moll mit grosser Septime, Cmmaj7, Cm(#)7
const byte SCALE_MMAJORSEPT_CHORD[] = {0,3,7,11};

// Septakkord mit kleiner None, C7/9-, C9-
const byte SCALE_SEPTNONEMINUS_CHORD[] = {0,4,7,10,13};

// Moll mit kleiner Septime und kleiner None, Cm7/9-
const byte SCALE_MSEPTNONEMINUS_CHORD[] = {0,3,7,10,13};

// -----------------------------------------------------------------------------

// export all: scales (8) + twelvetonemusic (1) + chords (24) = 33
extern const byte* CHORDS[] = {SCALE_MAJOR, SCALE_MINOR, SCALE_HARMONIC_MINOR, SCALE_MELODIC_MINOR, SCALE_MINOR_PENTATONIC, SCALE_MAJOR_PENTATONIC, SCALE_BLUES, SCALE_DIMINISHED, SCALE_TWELVETONE, SCALE_MAJOR_CHORD, SCALE_MINOR_CHORD, SCALE_SEPT_CHORD, SCALE_SEXT_CHORD, SCALE_NONE_CHORD, SCALE_DIM_CHORD, SCALE_AUG_CHORD, SCALE_FIVEMINUS_CHORD, SCALE_SUS_CHORD, SCALE_SEPTQUART_CHORD, SCALE_SEPTSEXT_CHORD, SCALE_SEPTNONE_CHORD, SCALE_SEXTNONE_CHORD, SCALE_MAJORSEPT_CHORD, SCALE_MAJORSEPTNONE_CHORD, SCALE_MSEXT_CHORD, SCALE_MSEPT_CHORD, SCALE_MSEPTNONE_CHORD, SCALE_MSEPTFIVEMINUS_CHORD, SCALE_SEPTAUG_CHORD, SCALE_SEPTFIVEMINUS_CHORD, SCALE_MMAJORSEPT_CHORD, SCALE_SEPTNONEMINUS_CHORD, SCALE_MSEPTNONEMINUS_CHORD};

extern const byte  CHORDS_LENGTH[] = {7,7,7,7,5,5,6,8,12,3,3,4,4,4,4,3,3,3,4,5,5,5,4,5,4,4,5,4,4,4,4,5,5};

// Names to display in alphanum display (4 characters)
extern const char* CHORDS_NAMES[] = {"Maj","min","mHar","mMel","mPen","MPen","Blue","Dim","Sc12","M","m","7","6","M9","dim0","aug5","5-","sus4","7/4","7/6","7/9","6/9","M7","M7/9","m6","m7","m7/9","m7-5","7+5","7-5","mm7","7/9-","m7/9"};


#endif // SCALES_H
