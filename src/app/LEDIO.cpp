#include "../SerialUSBHost.h"
#include "../../firmware/src/ProtocolConstants.h"

#include <iostream>
using namespace std;

int main() {
  SerialUSBHost host;

  host.sendRaw(ECHO);


  while (true) {
    int pin;
    int level;
    cin >> pin >> level;
    level = (bool)level;
    host.sendCmd({0, SET_PINMODE, 2, pin, 1});
    host.sendCmd({0, SET_DIGITAL, 2, pin, level});
  }
}
