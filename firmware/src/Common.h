#ifndef __COMMON_H__
#define __COMMON_H__

#include "WProgram.h"

#include <cstddef>

static const size_t MAX_PAYLOAD = 256;

typedef uint8_t u8;

inline u8 blockingSerialRead() {
  int data;
  while ((data = Serial.read()) == -1) {
    // loop until a character is received
  }
  return data;
}

#endif // __COMMON_H__
