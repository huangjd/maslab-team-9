#ifndef __COMMON_H__
#define __COMMON_H__

#include "WProgram.h"

#include <cstddef>

static const size_t MAX_PAYLOAD = 256;

typedef uint8_t u8;

inline u8 blockingSerialRead() {
  int data;
  while ((data = Serial.read()) == -1) {

  }
  return data;
}

inline void blockingSerialWrite(u8 c) {
  while (Serial.write(c) == 0) {
  }
}

inline void blockingSerialWrite(size_t size, const u8* c) {
  do {
    size_t bits_written = Serial.write(c, size);
    c += bits_written;
    size -= bits_written;
  } while (size > 0);
}

#endif // __COMMON_H__
