#include <vector>

#include "Arduino.h"

#include "Config.h"
#include "Common.h"
#include "CommandDispatcher.h"

CommandDispatcher dispatcher;

void setup() {
  yield();
  analogReadResolution(ADC_RESOLUTION);
  analogWriteResolution(PWM_RESOLUTION);
  analogWriteFrequency(FTM0PIN, PWM0_FREQUENCY);
  analogWriteFrequency(FTM1PIN, PWM1_FREQUENCY);
  analogWriteFrequency(FTM2PIN, PWM2_FREQUENCY);
}

void loop() {
  u8 seqnum = blockingSerialRead();
  switch (seqnum & SENDER_MASK) {
  case MASTER_SEND: {
    CommandType command = (CommandType) blockingSerialRead();
    dispatcher.process(seqnum & SEQNUM_MASK, command);
  }
    return;
  case MASTER_ACK:
    // ToDo: Process ack;

    return;
  case SLAVE_SEND:
    // Drop
  case SLAVE_ACK:
    // Drop
    ;
  }
}

int main() {
  setup();
  while(1) {
    loop();
  }
}
