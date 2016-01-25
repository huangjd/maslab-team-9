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
void CubeContours(const Mat& src, const Mat& src_binary_r, const Mat& src_binary_g, Mat& src_contour);

///Movement Functions to be implemented (fill in functions at bottom)
//Movement should be small, and so the function can be repeatedly called
void back_up_rotate(); //Should be broken up into two functions to 1) back up a certain distance and 2) rotate is still no cube is found
void move_forward();
void turn_left();
void turn_right();

//Set movement forward once block is in known position
void move_forward_to_red_block();
void move_forward_to_green_block();

vector<int> blockCenter_r_x, blockCenter_r_y, blockCenter_g_x, blockCenter_g_y;
int resX = 640;
int resY = 480;

///Main Function
int main( int argc, char** argv )
{
    ///Camera Initialization
    VideoCapture cap(0);        //Capture the video from web cam
    cap.open(true);             //Open camera stream
    Mat image, imageDrawn;      //Define matrix for image
    Mat src_bin_r, src_bin_g;
    int target_x, target_y;

    ///Check if camera is open
    if ( !cap.isOpened() )      // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }

    cap.set(CV_CAP_PROP_FRAME_WIDTH,resX);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,resY);

    int thresCanny = 130;
    int notFound = 0;

    ///Camera stream loop
    while (true)
    {
        cap>>image;             //Store camera livestream to frame

        ColorThresholding(image, thresCanny, src_bin_r, src_bin_g);
        CubeContours(image, src_bin_r, src_bin_g, imageDrawn);

        imshow("Camera Feed", imageDrawn);   //Display camera

        ///Actions to do if cube is found
        //Red cube has priority; first detected cube will be picked up

        ///If Red Cube is present (priority over green)
        //Goes to right stack
        //Moves to have cube with center at top right corner
        if(blockCenter_r_x.size() > 0)
        {
            notFound = 0;
            target_x = blockCenter_r_x[0];
            target_y = blockCenter_r_y[0];

            ///Fix the decimal values
            if((target_x > (resX - 0.25*resX))&&(target_y > 0.1*resY)&&(target_y <  0.25*resY)) //If cube is in corner
            {
                move_forward_to_red_block();
            }
            else if(target_y < 0.1*resY) //If cube is too far right
            {
                turn_right();
            }
            else if(target_y > 0.25*resY) //If cube is too far left
            {
                turn_left();
            }
            else if(target_x < (resX - 0.25*resX)) //If cube is too far forward
            {
                move_forward();
            }

            cout << "r: " << target_x << ", " << target_y;
        }
        ///If Green Cube is present and not red
        //Goes to right stack
        //Moves to have cube with center at bottom right corner
        else if(blockCenter_g_x.size() > 0)
        {
            notFound = 0;
            target_x = blockCenter_g_x[0];
            target_y = blockCenter_g_y[0];

            if((target_x > (resX - 0.25*resX))&&(target_y > (resY - 0.25*resY))&&(target_y < (resY - 0.1*resY))) //If cube is in corner
            {
                move_forward_to_green_block();
            }
            else if(target_y < (resY - 0.25*resY)) //If cube is too far right
            {
                turn_right();
            }
            else if(target_y > (resY - 0.1*resY)) //If cube is too far left
            {
                turn_left();
            }
            else if(target_x < (resX - 0.25*resX)) //If cube is too far forward
            {
                move_forward();
            }

            cout << "g: " << target_x << ", " << target_y;
        }
        else
        {
            notFound++;
            cout << notFound << endl;

            ///If no cube present (accounting for noise)
            if(notFound >= 20)
            {
                back_up_rotate(); //function to back up robot and start turning in place (direction doesn't matter)
            }
        }

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
            if((g >= 1.1*r) && (g >= 1*b))
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

}

///Algorithm to detect entire cube with contours
void CubeContours(const Mat& src, const Mat& src_binary_r, const Mat& src_binary_g, Mat& src_contour)
{
    src_contour = src.clone();

    vector<vector<Point> > contours_r, contours_g;
    vector<Vec4i> hierarchy_r, hierarchy_g;

    findContours( src_binary_r, contours_r, hierarchy_r, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    findContours( src_binary_g, contours_g, hierarchy_g, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    vector<RotatedRect> rect_r( contours_r.size() );
    vector<RotatedRect> rect_g( contours_g.size() );

    blockCenter_r_x.clear();
    blockCenter_r_y.clear();
    blockCenter_g_x.clear();
    blockCenter_g_y.clear();

    /// Draw contours

    for( int i = 0; i< contours_r.size(); i++ )
    {
        if(contourArea(contours_r[i]) > src.rows*src.cols*0.01)
        {
            Scalar color = Scalar(0, 0, 255);
            drawContours( src_contour, contours_r, i, color, 2, 8, hierarchy_r, 0, Point() );

            rect_r[i] = minAreaRect( Mat(contours_r[i]) );
            Point2f center = rect_r[i].center;

            if((center.x != 0)&&(center.y != 0))
            {
                blockCenter_r_x.push_back(center.x);
                blockCenter_r_y.push_back(center.y);
            }

            Point2f rect_points[4];
            rect_r[i].points( rect_points );
            for( int j = 0; j < 4; j++ )
                line( src_contour, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
        }
    }

    for( int i = 0; i< contours_g.size(); i++ )
    {
        if(contourArea(contours_g[i]) > src.rows*src.cols*0.01)
        {
            Scalar color = Scalar(0, 255, 0);
            drawContours( src_contour, contours_g, i, color, 2, 8, hierarchy_g, 0, Point() );

            rect_g[i] = minAreaRect( Mat(contours_g[i]) );
            Point2f center = rect_g[i].center;

            if((center.x != 0)&&(center.y != 0))
            {
                blockCenter_g_x.push_back(center.x);
                blockCenter_g_y.push_back(center.y);
            }

            Point2f rect_points[4];
            rect_g[i].points( rect_points );
            for( int j = 0; j < 4; j++ )
                line( src_contour, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
        }
    }
}

void back_up_rotate()
{
    cout << ": Cube not found - backing up and rotating." << endl;
}
void move_forward()
{
    cout << ": Going Forward" << endl;
}
void turn_left()
{
    cout << ": Turning Left" << endl;
}
void turn_right()
{
    cout << ": Turning Right" << endl;
}
void move_forward_to_red_block()
{
    cout << ": Moving to Red Block" << endl;
}
void move_forward_to_green_block()
{
    cout << ": Moving to Green Block" << endl;
}
