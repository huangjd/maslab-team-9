#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <cstdint>

#define SENDER_MASK 0b11000000
#define SEQNUM_MASK 0b00111111

#define MASTER_SEND 0b00000000
#define SLAVE_SEND  0b01000000
#define MASTER_ACK  0b10000000
#define SLAVE_ACK   0b11000000

enum CommandType {
  NOP = 0,
  ECHO,
  CPUID
};

enum Status {
  OK = 0,
  INVALID_SEQNUM,
  INVALID_COMMAND,
  INVALID_PIN_TYPE,
  PIN_EXCLUSIVE,
};

#endif // __COMMAND_H__
