#include "../CV.h"
#include "StackGet.h"
#include <iostream>
using namespace std;

bool getStack() {
    //Move towards stack
    Camera cam;
	while (true) {
		cam.moveTowardsStack();
		if(cam.stackFound == 1) //If stack is found
        {
            //Move towards individual cubes
            while (true) {
                cam.moveTowardsCube();
                if ((cam.cubeFound >= 3) || (cam.cubeNotFound > 100)) //If 3 cubes are found or timeout
                    return true;
            }

        }
        else if(cam.stackNotFound > 100) //time out
            return false;

	}
}
