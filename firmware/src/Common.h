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

template <typename T>
void sendBinary(const T& val) {
  Serial.write((const uint8_t*)(&val), sizeof(T));
}

template <typename T>
void recvBinary(T *val) {
  Serial.readBytes(val, sizeof(T));
}

#endif // __COMMON_H__
