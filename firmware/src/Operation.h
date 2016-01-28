#ifndef __OPERATION_H__
#define __OPERATION_H__

#include "Config.h"

void clock(int pin, int hold = 0, int d = 0);

void clampOperation(unsigned int mode);

void doorOperation(unsigned int mode);

void stepperOperation(unsigned int mode, int steps = STEPPER_STEP);

void emergencyBackUp();

#endif // __OPERATION_H__