#include "CommandDispatcher.h"

#include <vector>
#include <cstdint>

#include "Commands.h"

using namespace std;

typedef vector<uint8_t> Command;

CommandDispatcher::CommandDispatcher() {

}

void CommandDispatcher::process(u8 seqnum, CommandType command) {
  seqnum |= SLAVE_ACK;
  switch (command) {
  case NOP:
    ack(seqnum, OK);
    return;
  case ECHO: {
    u8 size = blockingSerialRead();
    in_buf[0] = size;
    for (u8 i = 1; i <= size; i++) {
      in_buf[i] = blockingSerialRead();
    }
    ack(seqnum, OK, size + 1, in_buf);
  }
    return;
  case CPUID:
    ack(seqnum, OK, {1, 2, 3, 4});
    return;
  default:
    ack(seqnum, INVALID_COMMAND);
    return;
  }
}

void CommandDispatcher::ack(u8 seqnum, Status status) {
  blockingSerialWrite(seqnum);
  blockingSerialWrite(status);
}

void CommandDispatcher::ack(u8 seqnum, Status status, u8 length, const u8 *buffer) {
  ack(seqnum, status);
  blockingSerialWrite(length, buffer);
}

void CommandDispatcher::ack(u8 seqnum, Status status, const vector<uint8_t> &payload) {
  ack(seqnum, status);
  blockingSerialWrite(payload.size(), &payload[0]);
}
