#ifndef __TRANSMISSION_LAYER_H__
#define __TRANSMISSION_LAYER_H__

#include <cstdint>
#include <cstddef>
#include "ProtocolConstants.h"

typedef uint8_t u8;

extern u8 recvBuf[RX_BUF_SIZE];
extern size_t recvBufIndex;

void clearRecvBuf();
void getCommandBlocked();

class Command;
void sendPacket(Command* cmd);

#endif // __TRANSMISSION_LAYER_H__
