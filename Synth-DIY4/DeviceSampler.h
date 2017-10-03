/** PCM Samples in PROGMEM (32kB)
*/

#pragma once

#include "PCM.h"

class DeviceSampler
{
public:
	DeviceSampler();
	~DeviceSampler();

	void playSample(void);

private:
	// TODO initialize in PROGMEM
	//const unsigned char sample;

};

