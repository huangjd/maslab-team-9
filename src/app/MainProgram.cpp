#include "../CV.h"
#include <iostream>
#include "gtest/gtest.h"
#include <sys/resource.h>
#include <string>
using namespace std;

#include "../SerialUSBHost.h"

extern USBProxy2 usbProxy;

int main(int argc, char **argv) {
    //Move towards stack
    Camera cam;
	while (true) {
		cam.moveTowardsStack();
		if(cam.stackFound == 1) //If stack is found
        {
            //Move towards individual cubes
            while (true) {
                cam.moveTowardsCube();
                if ((cam.cubeFound >= 3) || (cam.cubeNotFound > 100) //If 3 cubes are found or timeout
                    break;
            }

            break;
        }
        else if(cam.stackNotFound > 100) //time out
            break;

	}
}
