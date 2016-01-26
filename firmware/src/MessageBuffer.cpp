#include "WProgram.h"
#include "MessageBuffer.h"
#include "Commands.h"

#include <cstring>

char rxbuf[RX_MAX + 1];
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


bool MessageBuffer::dispatch(char cmd) {
  return commandsRegister[cmd - (char)64]();
}
