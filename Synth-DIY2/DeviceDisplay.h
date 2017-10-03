#pragma once

#include "Workstation.h"

class DeviceDisplay {
public:
	DeviceDisplay();
	~DeviceDisplay();

	void alphaWrite(char *);
	void alphaReadSerial(void);

private:

};
