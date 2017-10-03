/** alphanumeric Display
* 0.54", 14 segments, 4 letters
* I2C (Chip on PCB)
* https://learn.adafruit.com/adafruit-led-backpack/0-54-alphanumeric
* File > Examples > Adafruit_LEDBackpack > quadalphanum
*/

#ifndef DEVICE_DISPLAY_H
#define DEVICE_DISPLAY_H

#include "Workstation.h"

#define ADDR_ALPHA          0x70    //1110000
#define NUM_DIGITS             4    // TODO: alphanum display number of digits


class DeviceDisplay {
public:
	DeviceDisplay();
	~DeviceDisplay();

	void alphaWrite(char *);
	void alphaReadSerial(void);

	Adafruit_AlphaNum4 alpha4;

private:
	// data
	char* mDisplaybuffer;

	unsigned long mDelay;

};


#endif
