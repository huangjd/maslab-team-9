#include "../HAL.h"

#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
using namespace std;

int main() {
  string mode;
  int duration;
  while (true) {
    cout << "Input Direction: " << endl;
    getline(cin, mode);
    cout << "Input Duration: " << endl;
    cin >> duration;
    
    if(mode == "F")
        move_forward(duration);
    else if(mode == "T")
        turn(duration);

    cin.clear();
    cin.ignore();
  }
}
