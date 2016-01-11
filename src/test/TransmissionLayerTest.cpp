#include <csignal>
#include <csetjmp>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <vector>
#include <sys/resource.h>
#include <unistd.h>

#include <gtest/gtest.h>

#include "../../firmware/src/ProtocolConstants.h"

static uint32_t delayCounter = 0;
void delay(uint32_t ms) {
 delayCounter += ms;
}

extern uint8_t recvBuf[RX_BUF_SIZE];
extern size_t recvBufIndex;

class HackSerial {
  std::vector<uint8_t> input;
  std::vector<uint8_t> output;
  size_t index = 0;

public:
  void reset(const std::vector<uint8_t> &values) {
    input = values;
    output.clear();
    index = 0;
    recvBufIndex = 0;
  }

  const std::vector<uint8_t>& getOutput() const {
    return output;
  }

  int read() {
    if (index < input.size()) {
      uint8_t c = input[index];
      index++;
      return c;
    } else {
      return -1;
    }
  }

  size_t write(int n) {
    output.push_back((uint8_t)n);
  }

  void send_now() {

  }

} Serial;

#define __HACK_TRANSMISSION_LAYER_TEST_CPP__
#include "../../firmware/src/TransmissionLayer.cpp"

void release() {}
void drop() {}
void endgame() {}
void powerdown() {}
void terminate() {}
void halt() {}
void cpuid() {}

using namespace std;

template<class T>
static void assertVectorEqual(const vector<T>& expected, const vector<T>& actual) {
  ASSERT_EQ(expected.size(), actual.size());
  for (size_t i = 0; i < expected.size(); i++) {
    ASSERT_EQ(expected[i], actual[i]);
  }
}

static void assertRecvBuffer(const vector<uint8_t>& expected) {
  ASSERT_EQ(recvBufIndex, expected.size());
  for (size_t i = 0; i < expected.size(); i++) {
    ASSERT_EQ(expected[i], recvBuf[i]);
  }
}

jmp_buf env;

void sigxcpuHandler(int) {
  longjmp(env, 1);
}

void sigxcpuHandler_fail(int) {
  ADD_FAILURE();
  longjmp(env, 1);
}

void timedRun(void(*func)(), bool expectedReturn = false, int seconds = 1) {
  static int sec = 0;

  rlimit old_limit, new_limit;
  getrlimit(RLIMIT_CPU, &old_limit);
  new_limit.rlim_cur = (sec += seconds);
  new_limit.rlim_max = old_limit.rlim_max;

  if (!sigsetjmp(env, 1)) {
    if (expectedReturn) {
      signal(SIGXCPU, sigxcpuHandler_fail);
    } else {
      signal(SIGXCPU, sigxcpuHandler);
    }
    int i = setrlimit(RLIMIT_CPU, &new_limit);
    func();
  }
  setrlimit(RLIMIT_CPU, &old_limit);
}

void TrivialCtrlCharTest(Characters c) {
  Serial.reset({c});
  timedRun(getCommandBlocked);
  assertVectorEqual({}, Serial.getOutput());
  assertRecvBuffer({});
}

void InvalidAfterCmdCtrlCharTest(Characters c) {
  Serial.reset({CMD, c});
  timedRun(getCommandBlocked);
  assertVectorEqual({INVAL}, Serial.getOutput());
  assertRecvBuffer({});
}

void InvalidCtrlCharTest(Characters c) {
  Serial.reset({c});
  timedRun(getCommandBlocked);
  assertVectorEqual({INVAL}, Serial.getOutput());
  assertRecvBuffer({});
}



TEST(TransmissionLayerTest, NUL)  {
  Serial.reset({});
  timedRun(getCommandBlocked);
  assertVectorEqual({}, Serial.getOutput());
  ASSERT_EQ(recvBufIndex, 0);

  TrivialCtrlCharTest(NUL);
  InvalidAfterCmdCtrlCharTest(NUL);
}

TEST(TransmissionLayerTest, CAN) {
  TrivialCtrlCharTest(CAN);

  Serial.reset({CMD, CAN});
  timedRun(getCommandBlocked);
  assertVectorEqual({}, Serial.getOutput());
  ASSERT_EQ(recvBufIndex, 0);

  Serial.reset({CMD, ESC, CAN});
  timedRun(getCommandBlocked);
  assertVectorEqual({}, Serial.getOutput());
  ASSERT_EQ(recvBufIndex, 0);
}

TEST(TransmissionLayerTest, ACK) {
  InvalidCtrlCharTest(ACK);
  InvalidAfterCmdCtrlCharTest(ACK);
}

TEST(TransmissionLayerTest, ECHO)  {
  Serial.reset({ECHO, ECHO, NUL, ECHO, CMD, ECHO, ECHO, 128, REPR, ESC, ECHO, ECHO});
  timedRun(getCommandBlocked);
  assertVectorEqual({AECHO, AECHO, AECHO, AECHO, AECHO, INVAL, AECHO}, Serial.getOutput());
}

TEST(TransmissionLayerTest, AECHO) {
  TrivialCtrlCharTest(AECHO);
}

TEST(TransmissionLayerTest, INVAL) {
  TrivialCtrlCharTest(INVAL);
  InvalidAfterCmdCtrlCharTest(INVAL);
}

TEST(TransmissionLayerTest, TIMEOUT) {
  TrivialCtrlCharTest(TIMEOUT);
  InvalidAfterCmdCtrlCharTest(TIMEOUT);
}

TEST(TransmissionLayerTest, ESC) {
  InvalidCtrlCharTest(ESC);
}

TEST(TransmissionLayerTest, ECHOBUF) {
  Serial.reset({ECHOBUF, CMD, 50, ECHOBUF, REPR, ESC, ESCAPE_OFF + 1, 'a', ECHOBUF});
  timedRun(getCommandBlocked, true);
  assertVectorEqual({AECHOBUF, 0, 0, AECHOBUF, 1, 0, 50}, Serial.getOutput());
  timedRun(getCommandBlocked);
  assertVectorEqual({AECHOBUF, 0, 0, AECHOBUF, 1, 0, 50, AECHOBUF, 4, 0, 50, REPR, 1, 'a'}, Serial.getOutput());
}

TEST(TransmissionLayerTest, DELAY1)  {
  vector<uint8_t> vec(1000, DELAY1);
  Serial.reset(vec);
  timedRun(getCommandBlocked);
  assertVectorEqual({}, Serial.getOutput());
  ASSERT_EQ(recvBufIndex, 0);
  ASSERT_EQ(delayCounter, 1000);
}



TEST(TransmissionLayerTest, NonBlockingNUL) {
  Serial.reset({CMD, 32, REPR, 20, 'a', 'b', NUL});
  timedRun(getCommandBlocked, true);
  assertVectorEqual({}, Serial.getOutput());
  assertRecvBuffer({32, REPR, 20, 'a', 'b'});
}

TEST(TransmissionLayerTest, NonBlockingCMD) {
  Serial.reset({CMD, 32, REPR, ESC, ESCAPE_OFF + 5, 'a', 'b', ECHO, 'c', 'd', 'e'});
  timedRun(getCommandBlocked, true);
  assertVectorEqual({AECHO}, Serial.getOutput());
  assertRecvBuffer({32, REPR, 5, 'a', 'b', 'c', 'd', 'e'});

  Serial.reset({CMD, 'Y', 'V', ESC, ESCAPE_OFF,
                'a',
                CMD, 'A', 'W', 34, 'c', 'd', 'e', NUL,
                CMD, 'A', 'W', 34, CAN,
                CMD, 'B', 'X', ESC, ESCAPE_OFF + 3, ECHO, 'f', 'g', 'h', INVAL
               });
  timedRun(getCommandBlocked, true);
  assertVectorEqual({}, Serial.getOutput());
  assertRecvBuffer({'Y', 'V', 0});

  timedRun(getCommandBlocked, true);
  assertVectorEqual({INVAL}, Serial.getOutput());
  assertRecvBuffer({'A', 'W', 34, 'c', 'd', 'e'});

  timedRun(getCommandBlocked, true);
  assertVectorEqual({INVAL, AECHO}, Serial.getOutput());
  assertRecvBuffer({'B', 'X', 3, 'f', 'g', 'h'});
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
