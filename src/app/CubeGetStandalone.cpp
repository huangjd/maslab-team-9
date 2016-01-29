#include "../CV.h"
#include "../StackGet.h"
#include <iostream>
using namespace std;

int main()
{
  //Move towards stack
  Camera cam;
  while (true) {
    //cam.moveTowardsStack();
    cam.moveTowardsCube();
    if((cam.cubeFound == 3)||(cam.cubeNotFound >= 1000))  //If stack is found
    {
        exit(0);
    }
  }
  return 0;
}
