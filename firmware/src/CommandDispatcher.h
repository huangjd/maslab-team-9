#ifndef __COMMAND_DISPATCHER_H__
#define __COMMAND_DISPATCHER_H__

#include "Common.h"
#include "Commands.h"

#include <vector>

class CommandDispatcher {
  u8 in_buf[MAX_PAYLOAD];
  u8 out_buf[MAX_PAYLOAD];
public:
  CommandDispatcher();

  void process(u8 seqnum, CommandType command);

  void ack(u8 seqnum, Status status);
  void ack(u8 seqnum, Status status, u8 length, const u8* buffer);
  void ack(u8 seqnum, Status status, const std::vector<u8> &payload);
};

#endif // __COMMAND_DISPATCHER_H__
