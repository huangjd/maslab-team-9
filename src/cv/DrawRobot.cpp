#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

void GenXYRPos();
vector<double> xP, yP, rR;

///Main Function
int main( int argc, char** argv )
{
    Mat Grid = Mat::zeros( 100, 100, CV_8UC3 );
    Mat GridLg = Mat::zeros( 400, 400, CV_8UC3 );

    //Gen Map
    ifstream mapFile;
    mapFile.open("map.txt");

    string value;
    vector<int> Start;
    vector<vector<int> > Stacks;
    vector<vector<int> > Platform;
    vector<vector<int> > Walls;

    while ( mapFile.good() )
    {
        getline ( mapFile, value, ',' );

        if (value == "L")
        {
            getline ( mapFile, value, ',' );
            Start.push_back(atoi(value.c_str()));
            getline ( mapFile, value, '\n' );
            Start.push_back(atoi(value.c_str()));
        }
        else if (value == "W")
        {
            vector<int> curWall;

            getline ( mapFile, value, ',' );
            curWall.push_back(atoi(value.c_str()));
            getline ( mapFile, value, ',' );
            curWall.push_back(atoi(value.c_str()));
            getline ( mapFile, value, ',' );
            curWall.push_back(atoi(value.c_str()));
            getline ( mapFile, value, '\n' );
            curWall.push_back(atoi(value.c_str()));

            Walls.push_back(curWall);
        }
        else if (value == "P")
        {
            vector<int> curPlat;

            getline ( mapFile, value, ',' );
            curPlat.push_back(atoi(value.c_str()));
            getline ( mapFile, value, ',' );
            curPlat.push_back(atoi(value.c_str()));
            getline ( mapFile, value, ',' );
            curPlat.push_back(atoi(value.c_str()));
            getline ( mapFile, value, '\n' );
            curPlat.push_back(atoi(value.c_str()));

            Platform.push_back(curPlat);
        }
        else if (value == "S")
        {
            vector<int> curStack;

            getline ( mapFile, value, ',' );
            curStack.push_back(atoi(value.c_str()));
            getline ( mapFile, value, ',' );
            curStack.push_back(atoi(value.c_str()));

            getline ( mapFile, value, ',' );

            if(value == "R") // R = 0; G = 1
                curStack.push_back(0);
            else
                curStack.push_back(1);

            getline ( mapFile, value, ',' );
            if(value == "R") // R = 0; G = 1
                curStack.push_back(0);
            else
                curStack.push_back(1);

            getline ( mapFile, value, '\n' );
            if(value == "R") // R = 0; G = 1
                curStack.push_back(0);
            else
                curStack.push_back(1);

            Stacks.push_back(curStack);
        }
    }

    ///Print out vectors and draw
    Vec3b color;

    //Start
    cout << "Start: " << Start[0] << ", " << Start[1] << endl;
    color[0] = 255; color[1] = 0; color[2] = 0;
    Grid.at<Vec3b>(Point(Start[0]*10,Start[1]*10)) = color; // Blue

    //Walls
    for(int i = 0; i < Walls.size(); i++)
    {
        line(Grid, Point(Walls[i][0]*10, Walls[i][1]*10), Point(Walls[i][2]*10, Walls[i][3]*10), Scalar(255,255,255));
        cout << "Wall " << i << ": " << Walls[i][0] << ", "  << Walls[i][1] << ", "
                        << Walls[i][2] << ", "  << Walls[i][3] << endl;
    }
    //Platforms
    for(int i = 0; i < Platform.size(); i++)
    {
        line(Grid, Point(Platform[i][0]*10, Platform[i][1]*10), Point(Platform[i][2]*10, Platform[i][3]*10), Scalar(0,0,255));
        cout << "Platform " << i << ": " << Platform[i][0] << ", "  << Platform[i][1] << ", "
                        << Platform[i][2] << ", "  << Platform[i][3] << endl;
    }
    //Stacks
    for(int i = 0; i < Stacks.size(); i++)
    {
        color[0] = 0; color[1] = 255; color[2] = 0;
        Grid.at<Vec3b>(Point(Stacks[i][0]*10,Stacks[i][1]*10)) = color;
        cout << "Stack " << i << ": " << Stacks[i][0] << ", "  << Stacks[i][1] << ", "
                        << Stacks[i][2] << ", "  << Stacks[i][3] << ", " << Stacks[i][4] << endl;
    }

    GenXYRPos();
    int x0, x1, x2, y0, y1, y2;
    int tempx, tempy;
    double a;

    for(int i = 0; i < xP.size(); i++)
    {
        a = rR[i] * (M_PI/180);

        x0 = xP[i]*40; y0 = yP[i]*40;
        x1 = xP[i]*40; y1 = yP[i]*40 + 5;
        x2 = xP[i]*40; y2 = yP[i]*40 - 5;

        cout << x1 << ", " << y1 << ":" << x2 << ", " << y2 << endl;

        tempx = ((x1 - x0) * cos(a)) - ((y1 - y0) * sin(a)) + x0;
        tempy = ((x1 - x0) * sin(a)) + ((y1 - y0) * cos(a)) + y0;
        x1 = tempx; y1 = tempy;

        tempx = ((x2 - x0) * cos(a)) - ((y2 - y0) * sin(a)) + x0;
        tempy = ((x2 - x0) * sin(a)) + ((y2 - y0) * cos(a)) + y0;
        x2 = tempx; y2 = tempy;

        resize(Grid, GridLg, GridLg.size(), 0, 0, INTER_NEAREST);
        arrowedLine(GridLg, Point(x1, y1), Point(x2, y2), Scalar(255, 255, 0), 1);
        imshow("Grid Large", GridLg);
        waitKey();

    }
}

void GenXYRPos()
{
    xP.push_back(5); yP.push_back(5); rR.push_back(0);
    xP.push_back(5); yP.push_back(5); rR.push_back(5);
    xP.push_back(5); yP.push_back(5); rR.push_back(10);
    xP.push_back(5); yP.push_back(5); rR.push_back(15);
    xP.push_back(5); yP.push_back(5); rR.push_back(20);
    xP.push_back(5); yP.push_back(5); rR.push_back(25);
    xP.push_back(5); yP.push_back(5); rR.push_back(30);
    xP.push_back(5); yP.push_back(5); rR.push_back(35);
    xP.push_back(5); yP.push_back(5); rR.push_back(40);
    xP.push_back(5); yP.push_back(5); rR.push_back(45);

    xP.push_back(5.2); yP.push_back(4.8); rR.push_back(45);
    xP.push_back(5.4); yP.push_back(4.6); rR.push_back(45);
    xP.push_back(5.6); yP.push_back(4.4); rR.push_back(45);
    xP.push_back(5.8); yP.push_back(4.2); rR.push_back(45);
    xP.push_back(6.0); yP.push_back(4.0); rR.push_back(45);
}
