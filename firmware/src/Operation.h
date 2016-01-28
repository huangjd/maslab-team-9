#ifndef __OPERATION_H__
#define __OPERATION_H__

#include "Config.h"

void clock(int pin, int hold = 0, int d = 0);

void clampOperation(bool right, int angle);

void doorOperation(unsigned int mode);

void stepperOperation(bool right, bool down, int steps);

void emergencyBackUp();

void halt();

#endif // __OPERATION_H__
