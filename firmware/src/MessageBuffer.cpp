#include "WProgram.h"
#include "MessageBuffer.h"
#include "Commands.h"

#include <cstring>

char rxbuf[RX_MAX];
TXBuffer txbuf;

TXBuffer::TXBuffer() : buffer(buf + 1) {
}

void TXBuffer::send() {
  size_t len = strlen(buffer);
  buf[0] = len;
  buffer[len] = '\0';
  Serial.write(buf, buf[0] + 2);
  Serial.send_now();
}


Command* MessageBuffer::create(char cmd) {
  switch (cmd) {
  case 'A':
    return new AnalogWrite;
  case 'B':
    return new Blink;
  case 'D':
    return new DigitalWrite;

  case 'a':
    return new AnalogRead;
  case 'd':
    return new DigitalRead;
  case 'e':
    return new Echo;
  default:
    return nullptr;
  }
}
