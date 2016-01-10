#ifndef __TRANSMISSION_LAYER_H__
#define __TRANSMISSION_LAYER_H__

#include <cstdint>
#include <cstddef>
#include "ProtocolConstants.h"

typedef uint8_t u8;

extern u8 recvBuf[RX_BUF_SIZE];
extern u8 sendBuf[TX_BUF_SIZE];
extern size_t recvBufIndex;
extern size_t sendBufIndex;

u8 getRawBlocked();
void clearRecvBuf();
void getCommandBlocked();

void sendCharAndFlush(u8 c);

void ackCommand();

#endif // __TRANSMISSION_LAYER_H__
