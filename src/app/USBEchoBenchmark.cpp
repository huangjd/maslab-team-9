#include "../SerialUSBHost.h"
#include "../../firmware/src/ProtocolConstants.h"

#include <unistd.h>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <csignal>
#include <chrono>

sig_atomic_t state = 0;

using namespace std::chrono;

int main() {
  SerialUSBHost host;

  char *c = new char[100001];
  char *d = new char[100001];

  memset(c, ECHO,    100000);

  int repeat = 100;

  auto t1 = high_resolution_clock::now();
  for (int i = 0; i < repeat; i++) {
    host.sendCmd(1, c);
    //host.recv(1, d);
  }
  auto t2 = high_resolution_clock::now();

  double sec = duration_cast<duration<double>>(t2 - t1).count();
  printf("%d 1-bit x2 packets are sent in %f s, bit rate is %f bit/s\n", repeat, sec, repeat * 2/sec);

  t1 = high_resolution_clock::now();
  for (int i = 0; i < repeat; i++) {
    host.sendCmd(10, c);
   // host.recv(10, d);
  }
  t2 = high_resolution_clock::now();
  sec = duration_cast<duration<double>>(t2 - t1).count();
  printf("%d 10-bit x2 packets are sent in %f s, bit rate is %f bit/s\n", repeat, sec, repeat * 20/sec);

  t1 = high_resolution_clock::now();
  for (int i = 0; i < repeat; i++) {
    host.sendCmd(100, c);
    //host.recv(100, d);
  }
  t2 = high_resolution_clock::now();
  sec = duration_cast<duration<double>>(t2 - t1).count();
  printf("%d 100-bit x2 packets are sent in %f s, bit rate is %f bit/s\n", repeat, sec, repeat * 200/sec);

  t1 = high_resolution_clock::now();
  for (int i = 0; i < repeat; i++) {
    host.sendCmd(1000, c);
    //host.recv(1000, d);
  }
  t2 = high_resolution_clock::now();
  sec = duration_cast<duration<double>>(t2 - t1).count();
  printf("%d 100000-bit x2 packets are sent in %f s, bit rate is %f bit/s\n", repeat, sec, repeat * 2000/sec);
}
