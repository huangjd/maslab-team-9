#include "HAL.h"

#include <cstdarg>
#include <iostream>
#include <cmath>

#include "SerialUSBHost.h"

using namespace std;

void move_to(double delta_x, double delta_y) {
  double degree = atan2(delta_y, delta_x);
  turn(degree);
  move_forward(hypot(delta_x, delta_y));
}

void move_forward(double x) {
  char s[20];
  sprintf(s, "M %.3f", (double)x);
  usbProxy.sendCmd(s);
}

void turn(double deg) {
  char s [20];
  sprintf(s, "T %.3f", (double)deg);
  usbProxy.sendCmd(s);
}

void echo(const string &in, string &out) {
  volatile bool ok = false;
  usbProxy.sendCmd(string("e") + in, &ok, out);
  while (!ok) {
  };
}

void pickup(bool side) {
  if (side) {
    usbProxy.sendCmd("P 1");
  } else {
    usbProxy.sendCmd("P 0");
  }
}

void release(bool side) {
  if (side) {
    usbProxy.sendCmd("R 1");
  } else {
    usbProxy.sendCmd("R 0");
  }
}

void releaseToPlatform(bool side) {
  if (side) {
    usbProxy.sendCmd("R 3");
  } else {
    usbProxy.sendCmd("R 2");
  }
}

void halt() {
  usbProxy.sendCmd("H");
}
