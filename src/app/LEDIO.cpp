#include "../SerialUSBHost.h"

#include <iostream>
#include <string>
#include <unistd.h>
using namespace std;

int main() {
  SerialUSBHost host;

  {
    char recv[100];
    host.recvRaw(recv, 100);
    cout << recv << endl;
  }


  while (true) {
    string out;
    getline(cin, out);
    host.sendCmd(out);
    sleep(1);

    char recv[100];
    if (host.recvRaw(recv, 100)) {
      cout << recv << endl;
      recv[0] = '\0';
    }
  }
}
