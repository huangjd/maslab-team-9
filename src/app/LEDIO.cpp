#include "../SerialUSBHost.h"
#include "../../firmware/src/ProtocolConstants.h"

#include <iostream>
using namespace std;

int main() {
  SerialUSBHost host;

  host.sendRaw(ECHO);


  while (true) {
    char type;
    int pin;
    int level;
    cin >> type >> pin >> level;

    uint8_t lev = level;

    switch (type) {
    case 'A':
      host.sendCmd({0, SET_ANALOG, 2, pin, level});
      break;
    case 'D':
      host.sendCmd({0, SET_DIGITAL, 2, pin, (bool)level});
      break;
    case 'S':
      host.sendCmd({0, SET_PINMODE, 2, pin, level});
    }
  }
}
