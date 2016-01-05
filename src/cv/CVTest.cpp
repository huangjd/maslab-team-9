///Block Detection for MASLAB Team 9
///Author: Alan D. Cheng (adcheng@mit.edu)
///Description: The purpose of this program is to detect blocks to be stacked\
///Initial Test Build - 1/4/2016

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

void HoughDetection(const Mat& src, Mat& houghOut);

///Main Function
int main( int argc, char** argv )
{
    ///Camera Initialization
    VideoCapture cap(0);        //Capture the video from web cam
    cap.open(true);             //Open camera stream
    Mat image, imageDrawn;      //Define matrix for image

    ///Check if camera is open
    if ( !cap.isOpened() )      // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }

    ///Camera stream loop
    while (true)
    {
        cap>>image;             //Store camera livestream to frame

        HoughDetection(image, imageDrawn);

        imshow("Camera Feed", imageDrawn);   //Display camera frame

        if (waitKey(30) >= 0)   //Exit on keypress
            break;
    }

    return 0;
}

///Algorithm to detect holes w/ Canny and Hough
void HoughDetection(const Mat& src, Mat& houghOut)
{
    Mat src_gray, src_contours;
    int thresCanny = 130;
    houghOut = src.clone();


    /// Convert it to gray
    cvtColor( src, src_gray, CV_BGR2GRAY );

    /// Reduce the noise so we avoid false circle detection
    GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );
    Canny(src_gray, src_contours, thresCanny, 20);

    /// Find circles with Hough
    vector<Vec3f> circles;
    HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, thresCanny, 20, 0, 0 );

    /// Draw the circles detected
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // circle center
        circle( houghOut, center, 3, Scalar(0,255,0), -1, 8, 0 );
        // circle outline
        circle( houghOut, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }
    cout << circles.size() << endl;


    /// Show your results

    //imshow( "Source", houghOut );
    //imshow( "Gray", src_gray );
    imshow( "Contours", src_contours );
}
