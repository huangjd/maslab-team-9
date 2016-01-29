#include "../CV.h"
#include "../StackGet.h"
#include <iostream>
using namespace std;

int main()
{
  //Move towards stack
  while(true) {
  Camera cam;
  while (true) {
    //cam.moveTowardsStack();
    cam.moveTowardsCube();
     if(cam.cubeFound > 1) //If stack is found
       break;
     if(cam.cubeNotFound >= 1000)
    {
        exit(0);
    }
  }
  }
  return 0;
}
