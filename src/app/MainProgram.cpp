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
    cam.stackFound = 0;
	while (true) {
		cam.moveTowardsStack();
		if(cam.stackFound == 1) //If stack is found
            break;
	}
	//Move towards individual cubes
	cam.cubeFound = 0;
	while (true) {
		cam.moveTowardsCube();
		if (cam.cubeFound >= 3) //If 3 cubes are found
            break;
	}
}
