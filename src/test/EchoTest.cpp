#include "gtest/gtest.h"
#include <sys/resource.h>
#include <string>
using namespace std;

#include "../SerialUSBHost.h"

extern USBProxy2 usbProxy;

static bool vectorAnd(volatile bool val[], size_t n) {
  bool res = true;
  for (size_t i = 0; i < n; i++) {
    res = res && val[i];
  }
  return res;
}

constexpr int repeatn = 1000;
TEST(MassAsyncEcho, 0) {
  volatile bool ok[repeatn];
  string respond[repeatn];
  char buf[10];
  for (int i = 0; i < repeatn; i++) {
    sprintf(buf, "e %d", i);
    usbProxy.sendCmd(buf, &ok[i], respond[i]);
  }

  while (!vectorAnd(ok, repeatn));

  for (int i = 0; i < repeatn; i++) {
    ASSERT_EQ(i, stoi(respond[i]));
  }
}

TEST(Echo, string) {
  volatile bool ok;
  string resp;
  usbProxy.sendCmd("eA");
  usbProxy.sendCmd("eHello World!", &ok, resp);
  while (!ok) {

  }
  ASSERT_EQ(string("Hello World!"), resp);
}

TEST(EchoBlocked, string) {
  ASSERT_EQ(string("A"), usbProxy.sendCmdBlocked("eA"));
}

/*
TEST(AsyncEcho, str) {
  volatile bool a, b, c;
  string ack1, ack2, ack3;
  usbProxy.sendCmd("eA");
  usbProxy.sendCmd("eB");
  usbProxy.recvCmd(&a, &ack1);
  usbProxy.sendCmd("eC");
  usbProxy.recvCmd(&b, &ack2);
  usbProxy.recvCmd(&c, &ack3);

  while (! (a && b && c));

  ASSERT_EQ(string("A"), ack1);
  ASSERT_EQ(string("B"), ack2);
  ASSERT_EQ(string("C"), ack3);
}*/

int main(int argc, char **argv) {
  rlimit limit = {10, 20};
  setrlimit(RLIMIT_CPU, &limit);

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
