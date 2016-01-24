#ifndef __MESSAGE_BUFFER_H__
#define __MESSAGE_BUFFER_H__

#include "Config.h"

class TXBuffer {
  char buf[TX_MAX + 2];
public:
  char* const buffer;

  TXBuffer();
  void send();
};

extern char rxbuf[RX_MAX];
extern TXBuffer txbuf;

class Command;
class MessageBuffer {
public:
  static Command* create(char cmd);
};

#endif // __MESSAGE_BUFFER_H__
