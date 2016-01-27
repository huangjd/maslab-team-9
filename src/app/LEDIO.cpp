#include "../SerialUSBHost.h"

#include <iostream>
#include <string>
#include <unistd.h>
using namespace std;

int main() {
  while (true) {
    string cmd;
    getline(cin, cmd);
    cout << usbProxy.sendCmdBlocked(cmd) << endl;
  }
}
