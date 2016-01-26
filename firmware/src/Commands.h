#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <cstdint>
#include <cstdio>
#include "Config.h"

struct TXBuffer;

extern bool(*commandsRegister[64])();


struct Command {

  virtual ~Command();

  virtual bool process(const char *rxbuf, TXBuffer *txbuf) = 0;
};

struct Blink : public Command {

  bool process(const char *rxbuf, TXBuffer *txbuf);
};

struct Echo : public Command {
  char buf[RX_MAX];

  bool process(const char *rxbuf, TXBuffer *txbuf);

};

struct DigitalWrite : public Command {

  bool process(const char *rxbuf, TXBuffer *txbuf);
};

struct AnalogWrite : public Command {

  bool process(const char *rxbuf, TXBuffer *txbuf);
};

struct DigitalRead : public Command {
  int pin;
  int val;

  bool process(const char *rxbuf, TXBuffer *txbuf);

};

struct AnalogRead : public Command {
  int pin;
  int val;

  bool process(const char *rxbuf, TXBuffer *txbuf);

};

struct GyroCalibrate : public Command {

  bool process(const char *rxbuf, TXBuffer *txbuf);
};

struct GyroRead : public Command {

  bool process(const char *rxbuf, TXBuffer *txbuf);

};













struct Halt : public Command {
  int type;

  bool process(const char *rxbuf, TXBuffer *txbuf);

};

#endif // __COMMAND_H__
