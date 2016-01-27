#include "WProgram.h"
#include "MessageBuffer.h"
#include "Commands.h"

#include <cstring>

char dummy0[10];
char rxbuf[RX_MAX + 1];
TXBuffer txbuf;
char dummy1[10];

TXBuffer::TXBuffer() : buffer(buf + 2) {
}

void TXBuffer::send() {
  size_t len = strlen(buffer);
  buf[0] = '\x01';
  buf[1] = (char)len;
  buffer[len] = '\0';
  Serial.write(buf, buf[1] + 3);
  Serial.send_now();
}


bool MessageBuffer::dispatch() {
  char cmd = rxbuf[0];
  rxbuf[0] = ' ';
  return commandsRegister[cmd - (char)64]();
}
