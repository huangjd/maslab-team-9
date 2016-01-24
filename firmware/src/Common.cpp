#include "Common.h"

#include "WProgram.h"

u8 blockingSerialRead() {
  int data;
  while ((data = Serial.read()) == -1) {
    // loop until a character is received
  }
  return data;
}

bool verifyPin(int pin, PinType type) {
  return true;

  switch (type) {
  case DIGITAL:
    return pin >= 0 && pin < 34;

  case ANALOG:
    return pin >= 14 && pin <= 23 ||
           pin >= A10 && pin <= A13 ||
           pin == A14 ||
           pin >= 26 && pin <= 31;

  case PWM:
    return pin >= 3 && pin <= 6 ||
           pin >= 9 && pin <= 10 ||
           pin >= 20 && pin <= 23 ||
           pin == 25 ||
           pin == 32;

  case I2C:
    return pin == 18 || pin == 19;

  case SPI:
    return pin >= 10 && pin <= 13;

  default:
    return false;
  }
}
