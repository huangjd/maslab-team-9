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
		//	cv::waitKey();
		/*
		if(cam.stackFound == 1) //If stack is found
        {
            //Move towards individual cubes
            while (true) {
                cam.moveTowardsCube();
                if ((cam.cubeFound >= 3) || (cam.cubeNotFound > 100) || cv::waitKey(30)) //If 3 cubes are found or timeout
		  break;
		  }

		  }
		  else*/ if(cv::waitKey(30) >= 0) //time out
	  break;

	}

	return 0;
}
