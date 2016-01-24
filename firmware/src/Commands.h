#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <cstdint>
#include <cstdio>
#include "Config.h"
#include "MessageBuffer.h"

class Command {
public:
  virtual ~Command();

  virtual bool process(const char *rxbuf) = 0;
  virtual void respond(TXBuffer *txbuf);
};

class Blink : public Command {
public:
  bool process(const char *rxbuf);
};

class Echo : public Command {
  char buf[RX_MAX];
public:
  bool process(const char *rxbuf);
  void respond(TXBuffer *txbuf);
};

class DigitalWrite : public Command {
public:
  bool process(const char *rxbuf);
};

class AnalogWrite : public Command {
public:
  bool process(const char *rxbuf);
};

class DigitalRead : public Command {
  int pin;
  int val;
public:
  bool process(const char *rxbuf);
  void respond(TXBuffer *txbuf);
};

class AnalogRead : public Command {
  int pin;
  int val;
public:
  bool process(const char *rxbuf);
  void respond(TXBuffer *txbuf);
};

class CPUId : public Command {
public:
  bool process(const char *rxbuf);
  void respond(TXBuffer *txbuf);
};

class Halt : public Command {
  int type;
public:
  bool process(const char *rxbuf);
  void respond(TXBuffer *txbuf);
};

#endif // __COMMAND_H__
