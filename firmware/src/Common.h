#ifndef __COMMON_H__
#define __COMMON_H__

#include "WProgram.h"

#include <cstddef>

static const size_t MAX_PAYLOAD = 256;

typedef uint8_t u8;

u8 blockingSerialRead();

template <typename T>
void sendBinary(const T& val) {
  Serial.write((const uint8_t*)(&val), sizeof(T));
}

template <typename T>
void recvBinary(T *val) {
  Serial.readBytes(val, sizeof(T));
}

enum PinType {
  DIGITAL,
  ANALOG,
  PWM,
  I2C,
  SPI,
};

bool verifyPin(int pin, PinType type);

#endif // __COMMON_H__
