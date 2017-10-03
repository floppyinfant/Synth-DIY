/** PCM Samples in PROGMEM (32kB)
*/

#include "DeviceSampler.h"



DeviceSampler::DeviceSampler()
{
}


DeviceSampler::~DeviceSampler()
{
}


// save something in Program space
const unsigned char sample[] PROGMEM = {
	// TODO values 0 - 255
	0
};

void DeviceSampler::playSample() {
	startPlayback(sample, sizeof(sample));
}
