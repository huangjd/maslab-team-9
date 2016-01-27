#include "HAL.h"

#include <iostream>

#include "SerialUSBHost.h"

using namespace std;

void move_forward_to_red_block() {
    cout << ": Moving to Red Block" << endl;
}

void move_forward_to_green_block() {
    cout << ": Moving to Green Block" << endl;
}

void move_forward_to_block_stack()
{
    cout << ": Moving to Block Stack" << endl;
}

void turn_right() {
    cout << ": Turning Right" << endl;
}

void turn_left() {
    cout << ": Turning Left" << endl;
}

void move_forward() {
    cout << ": Going Forward" << endl;
}

void back_up_rotate() {
    cout << ": Cube not found - backing up and rotating." << endl;
}

void echo(const string &s) {
  string t = s;
  t.insert(s.begin(), 'e');
  usbProxy.sendCmd(t);
  volatile bool ok = false;
  string ack;
  usbProxy.recvCmd(&ok, &ack);
  while (!ok) {

  }
  cout << ack << endl;
}
