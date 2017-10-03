/** Rotary-Encoder
* Encoder + Pushbutton + RGB-LED
* https://www.circuitsathome.com/mcu/programming/reading-rotary-encoder-on-arduino
* http://www.circuitsathome.com/mcu/rotary-encoder-interrupt-service-routine-for-avr-micros
* https://code.google.com/p/m2tklib/wiki/t09
*/

#ifndef DEVICE_ENCODER_H
#define DEVICE_ENCODER_H

#include "Workstation.h"

#define ENC_PORT            PINB    //PINC for 14 (A0) + 15 (A1); PINB for 8 + 9; PIND for 0 + 1


class DeviceEncoder {
public:
	DeviceEncoder();
	~DeviceEncoder();

	int8_t readEncoder(void);
	void setRGBLED(byte, byte, byte);

	void demoRotary(void);

private:
	// data
	byte mEncoderValue;
	byte* mRGBLED; // RGB(A)

};


#endif
