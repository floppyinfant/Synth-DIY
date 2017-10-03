/**/

#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "Workstation.h"


/* *****************************************************************************
* Constants
* ************************************************************************** */

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


class Sequencer {
public:
	Sequencer(void);
	~Sequencer(void);

	void update();
	int getTime();
	int getStep();

	void updateSequencer(void);

	void startSequencer(void);
	void stopSequencer(void);
	void pauseSequencer(void);
	void playSequence(void);
	void recordSequence(void);

	
	// MIDI
	void handleClock(void);
	void handleStart(void);
	void handleStop(void);
	void handleContinue(void);
	void handleNoteOn(byte, byte, byte);
	void handleNoteOff(byte, byte, byte);
	void handleControlChange(byte, byte, byte);
	void handlePitchBend(byte, int);
	void handleProgramChange(byte, byte);

	
	
private:
	float getTimePerStepFromBpm(int);
	float mtof(byte);

};


/* STRUCT ******************************************************************* */

extern struct sequencer_t {

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

} sequencer;


#endif //SEQUENCER_H
