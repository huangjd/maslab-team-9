#include "../SerialUSBHost.h"

#include <iostream>


int main() {
  SerialUSBHost host;

  host.sendCmd("B 3");

  host.sendCmd("D 0 0");
  host.sendCmd("D 1 0");
  host.sendCmd("A 3 4");
  host.sendCmd("A 4 4");
}
