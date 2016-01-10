#ifndef __APPLICATION_LAYER_H__
#define __APPLICATION_LAYER_H__

#include "ProtocolConstants.h"

#include <cstddef>
#include <cstdint>

class Command;

typedef uint8_t u8;

extern u8 recvBuf[RX_BUF_SIZE];
extern u8 sendBuf[TX_BUF_SIZE];
extern size_t recvBufIndex;
extern size_t sendBufIndex;

int parseCommand();
void ackCommand(Command* cmd);

#endif // __APPLICATION_LAYER_H__
