///Block Detection for MASLAB Team 9
///Author: Alan D. Cheng (adcheng@mit.edu)
///Description: The purpose of this program is to detect blocks to be stacked
///Initial Test Build - 1/4/2016

#include <iostream>
#include <vector>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

void ColorThresholding(const Mat& src, int thresVal, Mat& thresOut);
void Thresholding(const Mat& src, int thresVal, Mat& thresOut);
void HoughDetection(const Mat& src, const Mat& src_gray, int thresCanny, Mat& houghOut);
void HoughLineDetection(const Mat& src, const Mat& src_binary, Mat& houghOut);

///Main Function
int main( int argc, char** argv )
{
    ///Camera Initialization
    VideoCapture cap(0);        //Capture the video from web cam
    cap.open(true);             //Open camera stream
    Mat image, imageDrawn;      //Define matrix for image
    Mat src_bin;

    ///Check if camera is open
    if ( !cap.isOpened() )      // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }

    cap.set(CV_CAP_PROP_FRAME_WIDTH,160);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,120);

    int thresCanny = 130;

    ///Camera stream loop
    while (true)
    {
        cap>>image;             //Store camera livestream to frame

        ColorThresholding(image, thresCanny, src_bin);
        //Thresholding(image, thresCanny, src_bin);
        //HoughLineDetection(image, src_bin, imageDrawn);
        //HoughDetection(image, src_bin, thresCanny, imageDrawn);

        imshow("Camera Feed", image);   //Display camera frame
        //imshow("Camera Feed", imageDrawn);   //Display camera frame
        //imshow("Binary Image", src_bin);

        if (waitKey(30) >= 0)   //Exit on keypress
            break;
    }

    return 0;
}

void ColorThresholding(const Mat& src, int thresVal, Mat& thresOut)
{
    Mat src_hsv, src_gray;
    //Mat bin_b, bin_g, bin_r, bin_out;
    vector<Mat> BGRchannels;

    Mat bin_r(src.size(), CV_8U);
    Mat bin_g(src.size(), CV_8U);

    int b,g,r;

    for(int i = 0; i < src.rows; i++)
    {
        for(int j = 0; j < src.cols; j++)
        {
            /// Get BGR values of each individual pixel
            b = src.at<cv::Vec3b>(i,j)[0];
            g = src.at<cv::Vec3b>(i,j)[1];
            r = src.at<cv::Vec3b>(i,j)[2];

            /// Filter for Red Pixels
            if((r >= 1.3*g) && (r >= 1.3*b))
                bin_r.at<uchar>(i,j) = 255;
            else
                bin_r.at<uchar>(i,j) = 0;

            /// Filter for Green Pixels
            if((g >= 1.3*r) && (g >= 1.3*b))
                bin_g.at<uchar>(i,j) = 255;
            else
                bin_g.at<uchar>(i,j) = 0;
        }
    }

    imshow("Red Binary", bin_r);
    imshow("Green Binary", bin_g);
/*

    /// Reduce the noise so we avoid false circle detection
    GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

    /// Apply Canny Edge Transformation before detecting with Hough
    Canny(src_gray, thresOut, thresVal, 20);
    imshow("Bin Before", thresOut);

    bitwise_and(thresOut, bin_out, thresOut);*/

}

///Algorithm to threshold image
void Thresholding(const Mat& src, int thresVal, Mat& thresOut)
{
    Mat src_gray;

    cvtColor(src, src_gray, CV_BGR2GRAY);

    /// Reduce the noise so we avoid false circle detection
    GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

    /// Apply Canny Edge Transformation before detecting with Hough
    Canny(src_gray, thresOut, thresVal, 20);
}

///Algorithm to detect holes with Hough
void HoughDetection(const Mat& src, const Mat& src_binary, int thresCanny, Mat& houghOut)
{
    Mat src_contours;
    houghOut = src.clone();

    /// Find circles with Hough
    vector<Vec3f> circles;
    HoughCircles( src_binary, circles, CV_HOUGH_GRADIENT, 1, src_binary.rows/8, thresCanny, 20, 0, 0 );

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
}

///Algorithm to detect straight lines with Hough
void HoughLineDetection(const Mat& src, const Mat& src_binary, Mat& houghOut)
{
    houghOut = src.clone();

    vector<Vec4i> lines;
    HoughLinesP(src_binary, lines, 1, CV_PI/180, 50, 50, 10 );

    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line( houghOut, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
    }
}
