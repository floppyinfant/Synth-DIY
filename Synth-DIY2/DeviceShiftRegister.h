#pragma once

#include "Workstation.h"

class DeviceShiftRegister {
public:
	DeviceShiftRegister();
	~DeviceShiftRegister();

	void clearRegisters(void);
	void writeRegisters(void);
	void setRegister(int, int);
	void demoShift(void);

private:

};