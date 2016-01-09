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

void ColorThresholding(const Mat& src, int thresVal, Mat& thresOut_r, Mat& thresOut_g);
void Thresholding(const Mat& src, int thresVal, Mat& thresOut);
void HoughDetection(const Mat& src, const Mat& src_gray, int thresCanny, Mat& houghOut);
void HoughLineDetection(const Mat& src, const Mat& src_binary, Mat& houghOut);
void CubeContours(const Mat& src, const Mat& src_binary_r, const Mat& src_binary_g, Mat& src_contour);

///Main Function
int main( int argc, char** argv )
{
    ///Camera Initialization
    VideoCapture cap(0);        //Capture the video from web cam
    cap.open(true);             //Open camera stream
    Mat image, imageDrawn;      //Define matrix for image
    Mat src_bin_r, src_bin_g;

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

        ColorThresholding(image, thresCanny, src_bin_r, src_bin_g);
        CubeContours(image, src_bin_r, src_bin_g, imageDrawn);
        //Thresholding(image, thresCanny, src_bin);
        //HoughLineDetection(image, src_bin, imageDrawn);
        //HoughDetection(image, src_bin, thresCanny, imageDrawn);

        imshow("Camera Feed", imageDrawn);   //Display camera frame
        //imshow("Camera Feed", imageDrawn);   //Display camera frame
        //imshow("Binary Image", src_bin);

        if (waitKey(30) >= 0)   //Exit on keypress
            break;
    }

    return 0;
}

void ColorThresholding(const Mat& src, int thresVal, Mat& thresOut_r, Mat& thresOut_g)
{
    Mat src_gray, src_bin;
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
    thresOut_r = bin_r;
    thresOut_g = bin_g;

    erode(bin_g, bin_g, Mat());
    dilate(bin_g, bin_g, Mat());

    erode(bin_r, bin_r, Mat());
    dilate(bin_r, bin_r, Mat());


    imshow("Red Binary", bin_r);
    imshow("Green Binary", bin_g);

    /*
    cvtColor(src, src_gray, CV_BGR2GRAY);

    /// Reduce the noise so we avoid false circle detection
    GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

    /// Apply Canny Edge Transformation before detecting with Hough
    Canny(src_gray, src_bin, thresVal, 20);

    bitwise_and(src_bin, bin_r, thresOut_r);
    bitwise_and(src_bin, bin_g, thresOut_g);

    imshow("Output Red", thresOut_r);
    imshow("Output Green", thresOut_g);*/

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

///Algorithm to detect entire cube with contours
void CubeContours(const Mat& src, const Mat& src_binary_r, const Mat& src_binary_g, Mat& src_contour)
{
    src_contour = src.clone();

    vector<vector<Point> > contours_r, contours_g;
    vector<Vec4i> hierarchy_r, hierarchy_g;

    findContours( src_binary_r, contours_r, hierarchy_r, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    findContours( src_binary_g, contours_g, hierarchy_g, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// Draw contours

    for( int i = 0; i< contours_r.size(); i++ )
    {
        if(contourArea(contours_r[i]) > src.rows*src.cols*0.01)
        {
            Scalar color = Scalar(0, 0, 255);
            drawContours( src_contour, contours_r, i, color, 2, 8, hierarchy_r, 0, Point() );
        }
    }

    for( int i = 0; i< contours_g.size(); i++ )
    {
        if(contourArea(contours_g[i]) > src.rows*src.cols*0.01)
        {
            Scalar color = Scalar(0, 255, 0);
            drawContours( src_contour, contours_g, i, color, 2, 8, hierarchy_g, 0, Point() );
        }
    }
}
