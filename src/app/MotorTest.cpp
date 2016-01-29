#include "../HAL.h"

#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
using namespace std;

int main() {
  while (true) {
    string mode;
    int duration;
    cout << "Input Direction: " << endl;
    getline(cin, mode);
    cout << "Input Duration: " << endl;
    cin >> duration;
    switch(mode)
    {
    case "F":
        move_forward(duration);
        break;
    case "T":
        turn(duration);
        break;
    default:
        break;
    }

  }
}
