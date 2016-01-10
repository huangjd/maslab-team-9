#include "../SerialUSBHost.h"
#include "../../firmware/src/ProtocolConstants.h"

#include <iostream>
using namespace std;

int main() {
  SerialUSBHost host;

  host.sendRaw(ECHO);

  uint8_t res;
  int status = host.recvRaw(&res);
  if (status) {
    cout << "OK" <<endl;
  }

  while (true) {
    uint8_t pin;
    uint8_t level;
    cin >> pin >> level;

    host.sendCmd({0, SET_PINMODE, 2, pin, 1});
    host.sendCmd({0, SET_ANALOG, 2, pin, level});
  }

  host.sendCmd({0, SET_PINMODE, 2, 13, 1});

   status = host.recvRaw(&res);
  if (status) {
    cout << "cmd = " << (int)res << endl;
  }

   status = host.recvRaw(&res);
  if (status) {
    cout << "size = " << (int)res << endl;
  }

  while (1) {
    char c;
    cin >> c;
    host.sendCmd({1, SET_DIGITAL, 2, 13, 1});
    cin >> c;
    host.sendCmd({1, SET_DIGITAL, 2, 13, 0});
  }


}
