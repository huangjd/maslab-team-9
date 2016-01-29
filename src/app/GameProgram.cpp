#include "../CV.h"
#include "../StackGet.h"
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include "../HAL.h"
using namespace std;

int main(){
    Camera cam;
    while (true)
    {
        srand (time(NULL));
        cam.stackNotFound = 0;
        cam.stackFound = 0;
        cam.cubeNotFound = 0;
        cam.cubeFound = 0;

        cam.gameMode = 0; // 0 = red; 1 = green

        while (true)
        {
            cam.moveTowardsStack();
            int v1 = rand() % 1000;
            if((cam.stackFound >= 1)||(cam.stackNotFound >= 300)||(v1==1000))
               break;
        }
        if(cam.stackFound >= 1)
        {
            while (true)
            {
                cam.moveTowardsCube();
                if(cam.cubeNotFound >= 300)
                    break;
            }
        }

        randomMovement();
    }
    return 0;
}
