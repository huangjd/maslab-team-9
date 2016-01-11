#ifndef __PROTOCOL_CONSTANTS_H__
#define __PROTOCOL_CONSTANTS_H__

#define RX_BUF_SIZE 520
#define TX_BUF_SIZE 521

enum Characters {
  TRANSMISSION_CONTROL = 0,
  NUL = TRANSMISSION_CONTROL,
  CAN,
  CMD,
  STATUS,
  ACK,
  ECHO,
  AECHO,
  INVAL,
  TIMEOUT,
  ESC,
  ECHOBUF,
  AECHOBUF,
  DELAY1,
  CLR,
  TERM,
  KILL,
  CPUID,

  MAX_TRANSMISSION_CONTROL,

  COMMAND = 32,
  REPR = COMMAND,
  DELAY,
  DELAY_NOACK,
  HALT,
  RELEASE,
  GET_CPUID,

  SET_PINMODE = 48,
  GET_DIGITAL,
  SET_DIGITAL,
  GET_ANALOG,
  SET_ANALOG,
  GET_PINS,
  SET_PINS,

  ESCAPE_OFF = 128,
  ACK_STATUS = ESCAPE_OFF,
  OK = ACK_STATUS,
  BAD_CMD = OK | 1,
  BAD_PERM = OK | 2,
  BAD_ARGS = OK | 4
};

enum AttrMask {
  C = 1,
  M = 2,
  S = 4,
  A = 8,
  R = 16,
};

const unsigned char CharacterAttr[256]  = {
  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 16
  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0, // 32
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,                                        // 48
  C|M|A|R,     C|M|A|R,     C|M|A|R,     C|M|A|R,

};

#endif // __PROTOCOL_CONSTANTS_H__
