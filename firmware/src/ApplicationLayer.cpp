#include "ApplicationLayer.h"
#include "Commands.h"

#include "WProgram.h"

static int ackNull(u8 seqnum, Characters status) {
  sendBuf[0] = seqnum;
  sendBuf[1] = status;
  sendBuf[2] = 0;
  sendBufIndex = 3;
  return 1;
}

static int ackNull(u8 seqnum, Characters status, u8 payloadSize) {
  sendBuf[0] = seqnum;
  sendBuf[1] = status;
  sendBuf[2] = payloadSize;
  sendBufIndex = payloadSize + 3;
  return 1;
}

int parseCommand() {
  if (recvBufIndex < 3) {
    Serial.write(INVAL);
    return 0;
  }

  u8 seqnum = recvBuf[0];
  u8 c = recvBuf[1];
  size_t size = recvBufIndex - 3;

  if (!CharacterAttr[c] & C) {
    return ackNull(seqnum, BAD_CMD);
  }
  if (!CharacterAttr[c] & M) {
    return ackNull(seqnum, BAD_PERM);
  }

  switch (c) {
  case REPR:
  case DELAY:
  case DELAY_NOACK:
  case HALT:
  case RELEASE:
  case CPUID:

  case SET_PINMODE:
    if (size == 2) {
      switch (recvBuf[4]) {
      case 0:
      case 1:
      case 2:
        pinMode(recvBuf[3], recvBuf[4]);
        return ackNull(seqnum, OK);
      default:
        return ackNull(seqnum, BAD_ARGS);
      }
    }
    break;

  case GET_DIGITAL:
    if (size == 1) {
      ackNull(seqnum, OK, 1);
      sendBuf[3] = digitalRead(recvBuf[3]);
      return 1;
    }
    break;

  case SET_DIGITAL:
    if (size == 2) {
      digitalWrite(recvBuf[3], recvBuf[4]);
      return ackNull(seqnum, OK);;
    }
    break;

  case GET_ANALOG:
    if (size == 1) {
      ackNull(seqnum, OK, 1);
      sendBuf[3] = analogRead(recvBuf[3]);
    }
    break;

  case SET_ANALOG:
    if (size == 2) {
      analogWrite(recvBuf[3], recvBuf[4]);
    }
    break;

  default:
    ;
  }
  return ackNull(seqnum, BAD_CMD);
}
