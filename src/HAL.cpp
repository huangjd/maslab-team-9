#include "HAL.h"

#include <cstdarg>
#include <iostream>
#include <cmath>

#include "SerialUSBHost.h"

using namespace std;

void move_to(double delta_x, double delta_y) {
  double degree = atan2(delta_y, delta_x);
  turn(degree / 3.1416 * 180);
  move_forward(hypot(delta_x, delta_y));
}

void move_forward(int x) {
  char s[20];
  sprintf(s, "M %d", x * 18);
  usbProxy.sendCmd(s);
}

void move_forward_custom_speed(int x, int power) {
  char s[20];
  sprintf(s, "F %d %d", x * 18, power);
  usbProxy.sendCmd(s);
}

void turn(int deg) {
  char s [20];
  sprintf(s, "T %d", deg);
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

void charge() {
  usbProxy.sendCmd("K");
}

void halt() {
  usbProxy.sendCmd("H");
}
