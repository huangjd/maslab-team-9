#include "../CV.h"
#include "../StackGet.h"
#include <iostream>
using namespace std;

int main()
{
  //Move towards stack
  Camera cam;
  while (true) {
    cam.moveTowardsStack();
    //cam.moveTowardsCube();
    if((cam.stackFound == 1)||(cam.stackNotFound >= 1000))  //If stack is found
      {
	exit(0);
      }
      }
  return 0;
}
