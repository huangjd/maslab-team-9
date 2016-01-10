#include "TransmissionLayer.h"

#ifndef __HACK_TRANSMISSION_LAYER_TEST_CPP__
#include "WProgram.h"
#endif

u8 recvBuf[RX_BUF_SIZE];
size_t recvBufIndex = 0;
// int timeout

u8 getRawBlocked() {
  int c;
  while ((c = Serial.read()) < 0) { // loop until a character arrives
  }
  return c;
}

void clearRecvBuf() {
  recvBufIndex = 0;
}

void getCommandBlocked() {
  int state = 0;
  while (1) {
    u8 c = getRawBlocked();
    switch (c) {
    case NUL:
      if (state > 3) {
        return;
      } else if (state > 0) {
        Serial.write(INVAL);
        state = 0;
      }
      break;

    case CAN:
      state = 0;
      break;

    case CMD:
      if (state > 0) {
        Serial.write(INVAL);
      }
      clearRecvBuf();
      state = 1;
      break;

  /*  //case ACK:
      Serial.write(INVAL);
      state = 0;
      break;*/

    case ECHO:
      Serial.write(AECHO);
      break;

    case AECHO:
      break;

    case INVAL:
    case TIMEOUT:
      if (state > 0) {
    case ACK:
        Serial.write(INVAL);
        state = 0;
      }
      break;

    case ESC:
      if (state > 0) {
        c = getRawBlocked();
        if (c >= ESCAPE_OFF) {
          recvBuf[recvBufIndex] = c - (u8)ESCAPE_OFF;
          recvBufIndex++;
          state++;
          if (state > recvBuf[2] + 3) {
            return;
          }
          break;
        } else if (c != CAN) {
          Serial.write(INVAL);
        }
        state = 0;
        break;
      } else {
        Serial.write(INVAL);
        state = 0;
      }
      break;

    case ECHOBUF:
      Serial.write((u8)recvBufIndex);
      Serial.write((u8)(recvBufIndex >> 8));
      for (size_t j = 0; j < recvBufIndex; j++) {
        Serial.write(recvBuf[j]);
      }
      break;

    case DELAY1:
      delay(1);
      break;

    case CLR:
      if (state > 0) {
        Serial.write(INVAL);
        state = 0;
      } else {
        clearRecvBuf();
      }
      break;

    default:
      if (state > 0) {
        recvBuf[recvBufIndex] = c;
        recvBufIndex++;
        state++;
        if (state > recvBuf[2] + 3) {
          return;
        }
      } else {
        Serial.write(INVAL);
        state = 0;
      }
      break;
    }
  }
}

void sendPacket(Command* cmd) {

}
