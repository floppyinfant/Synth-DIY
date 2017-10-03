/**
 * Adafruit Trellis
 * Button-Array + LED-Array using I2C
 * https://learn.adafruit.com/adafruit-trellis-diy-open-source-led-keypad/overview
 * https://github.com/adafruit/Adafruit_Trellis_Library
 * Examples
 * https://learn.adafruit.com/mini-untztrument-3d-printed-midi-controller
 * https://learn.adafruit.com/trellis-3d-printed-enclosure
 */

#ifndef DEVICE_TRELLIS_H
#define DEVICE_TRELLIS_H

#include "Workstation.h"


#define ADDR_TRELLIS        0x72    //set jumper A1; 1110000 - 1110111 (select jumpers A0,A1,A2)

#define NUM_TRELLIS            1
#define NUM_TRELLIS_BUTTONS    (NUM_TRELLIS * 16)

#define MOMENTARY              0
#define LATCHING               1
#define MODE                   LATCHING    // set the mode here

// UI (Trellis Buttons)
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


class DeviceTrellis {
public:
	DeviceTrellis();
	~DeviceTrellis();

	void trellisReadButtons(void);
	void trellisWrite(byte[]);
	void trellisDemo(void);

private:
	Adafruit_Trellis matrix0 = Adafruit_Trellis();
	Adafruit_TrellisSet trellis = Adafruit_TrellisSet(&matrix0);
};

#endif // !DEVICE_TRELLIS_H
