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
  buffer[len] = '\n';
  buffer[len+1] = '\0';
  Serial.write(buf, buf[1] + 4);
  Serial.send_now();
  buffer[0] = '\0';
}


bool MessageBuffer::dispatch() {
  char cmd = rxbuf[0];
  if (cmd >= (char)64 && cmd < (char)128) {
    rxbuf[0] = ' ';
    return commandsRegister[cmd - (char)64]();
  } else {
    return false;
  }
}
