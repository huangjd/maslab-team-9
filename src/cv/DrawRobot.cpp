#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

void GenXYRPos(vector<int> xPos, vector<int> yPos, vector<int> rRot);

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

    resize(Grid, GridLg, GridLg.size(), 0, 0, INTER_NEAREST);
    imshow("Grid Large", GridLg);
    waitKey();

}
