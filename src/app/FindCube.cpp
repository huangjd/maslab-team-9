#include "../CV.h"

#include <iostream>

using namespace std;
int main(int argc, char** argv) {
	Camera cam;
	while (true) {
		cam.moveTowardsCube();
		if (cv::waitKey(30) >= 0)   //Exit on keypress
	            break;
	}

	return 0;
}

