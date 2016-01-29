#include "CV.h"

#define SHOW_COUT

#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

void ColorThresholding(const cv::Mat& src, int thresVal, cv::Mat& thresOut_r, cv::Mat& thresOut_g) {
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

#ifndef NDEBUG
    //    imshow("Red Binary", bin_r);
    //    imshow("Green Binary", bin_g);
#endif
}

void CubeContours(const cv::Mat& src, const cv::Mat& src_binary_r, const cv::Mat& src_binary_g, cv::Mat& src_contour, std::vector<cv::Point2f> &blockCenter_r, std::vector<cv::Point2f> &blockCenter_g) {
    src_contour = src.clone();

    vector<vector<Point> > contours_r, contours_g;
    vector<Vec4i> hierarchy_r, hierarchy_g;

    findContours( src_binary_r, contours_r, hierarchy_r, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    findContours( src_binary_g, contours_g, hierarchy_g, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    vector<RotatedRect> rect_r( contours_r.size() );
    vector<RotatedRect> rect_g( contours_g.size() );

    blockCenter_r.clear();

    blockCenter_g.clear();


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
                blockCenter_r.push_back(center);
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
                blockCenter_g.push_back(center);
            }

            Point2f rect_points[4];
            rect_g[i].points( rect_points );
            for( int j = 0; j < 4; j++ )
                line( src_contour, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
        }
    }
}

void StackContours(const cv::Mat& src, const cv::Mat& src_binary_r, const cv::Mat& src_binary_g, cv::Mat& src_contour, cv::Point2f &block) {
    src_contour = src.clone();

    block.x = 0;
    block.y = 0;

    vector<vector<Point> > contours_r, contours_g;
    vector<Vec4i> hierarchy_r, hierarchy_g;

    findContours( src_binary_r, contours_r, hierarchy_r, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    findContours( src_binary_g, contours_g, hierarchy_g, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    vector<RotatedRect> rect_r( contours_r.size() );
    vector<RotatedRect> rect_g( contours_g.size() );

    Point2f bottom, bcorner1, bcorner2; //To find bottom-most center point

    /// Draw contours

    for( int i = 0; i< contours_r.size(); i++ )
    {
        if(contourArea(contours_r[i]) > src.rows*src.cols*0.01)
        {
            Scalar color = Scalar(0, 0, 255);
            drawContours( src_contour, contours_r, i, color, 2, 8, hierarchy_r, 0, Point() );

            rect_r[i] = minAreaRect( Mat(contours_r[i]) );

            Point2f rect_points[4], corner1, corner2;
            rect_r[i].points( rect_points );

            corner1.x = 0;
            corner1.y = 0;

            int index, indexA, indexB;
            //Find bottom line point
            for( int j = 0; j < 4; j++ )
            {
                if(corner1.x < rect_points[j].x)
                {
                    corner1 = rect_points[j];
                    index = j;
                }
                line( src_contour, rect_points[j], rect_points[(j+1)%4], color, 1, 8 ); //Draw rectangle
            }

            if(index == 0)
            {
                indexA = 1; indexB = 3;
            }
            else if(index == 3)
            {
                indexA = 0; indexB = 2;
            }
            else
            {
                indexA = index++; indexB = index--;
            }

            if(rect_points[indexA].x > rect_points[indexB].x)
                corner2 = rect_points[indexA];
            else
                corner2 = rect_points[indexB];

            bottom.x = abs((corner2.x + corner1.x)/2);
            bottom.y = abs((corner2.y + corner1.y)/2);

            if(bottom.x > block.x) //Find the bottom-most block
            {
                block = bottom;
                bcorner1 = corner1;
                bcorner2 = corner2;
            }
        }
    }

    for( int i = 0; i< contours_g.size(); i++ )
    {
        if(contourArea(contours_g[i]) > src.rows*src.cols*0.01)
        {
            Scalar color = Scalar(0, 255, 0);
            drawContours( src_contour, contours_g, i, color, 2, 8, hierarchy_g, 0, Point() );

            rect_g[i] = minAreaRect( Mat(contours_g[i]) );

            Point2f rect_points[4], corner1, corner2;
            rect_g[i].points( rect_points );

            corner1.x = 0;
            corner1.y = 0;

            int index, indexA, indexB;
            //Find bottom line point
            for( int j = 0; j < 4; j++ )
            {
                if(corner1.x < rect_points[j].x)
                {
                    corner1 = rect_points[j];
                    index = j;
                }
                line( src_contour, rect_points[j], rect_points[(j+1)%4], color, 1, 8 ); //Draw rectangle
            }

            if(index == 0)
            {
                indexA = 1; indexB = 3;
            }
            else if(index == 3)
            {
                indexA = 0; indexB = 2;
            }
            else
            {
                indexA = index++; indexB = index--;
            }

            if(rect_points[indexA].x > rect_points[indexB].x)
                corner2 = rect_points[indexA];
            else
                corner2 = rect_points[indexB];

            bottom.x = abs((corner2.x + corner1.x)/2);
            bottom.y = abs((corner2.y + corner1.y)/2);

            if(bottom.x > block.x) //Find the bottom-most block
            {
                block = bottom;
                bcorner1 = corner1;
                bcorner2 = corner2;
            }
        }
    }
    line( src_contour, bcorner1, bcorner2, Scalar(0,215,255), 4, 8 ); //Draw bottom line

}

Camera::Camera() : resX(320), resY(240), cap(0), thresCanny(130), cubeNotFound(0), stackNotFound(-50) {
  cap.open(0);
  if (!cap.isOpened()) {
    cout << "Cannot open the web cam" << endl;
    throw exception();
  }
  cap.set(CV_CAP_PROP_FRAME_WIDTH,resX);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT,resY);
}

Camera::~Camera() {
}

void Camera::moveTowardsCube() {
        Mat image;
        Mat imageDrawn;
        Mat src_bin_r;
        Mat src_bin_g;
	vector<Point2f> blockCenter_r;
	vector<Point2f> blockCenter_g;

	cap>>image;             //Store camera livestream to frame

	ColorThresholding(image, 130, src_bin_r, src_bin_g);
	CubeContours(image, src_bin_r, src_bin_g, imageDrawn, blockCenter_r, blockCenter_g);

#ifndef NDEBUG
      	imshow("Camera Feed", imageDrawn);   //Display camera
#endif

	///Actions to do if cube is found
        //Red cube has priority; first detected cube will be picked up

        ///If Red Cube is present (priority over green)
        //Goes to right stack
        //Moves to have cube with center at top right corner
        if(blockCenter_r.size() > 0)
        {
            cubeNotFound = 0;
            Point2f target = blockCenter_r[0];

            ///Fix the decimal values
            if((target.x > (resX - 0.25*resX))&&(target.y > 0.1*resY)&&(target.y <  0.25*resY)) //If cube is in corner
            {
                move_forward(1);
                cubeFound++;
            }
            else if(target.y < 0.1*resY) //If cube is too far right
            {
                turn(1);
            }
            else if(target.y > 0.25*resY) //If cube is too far left
            {
                turn(-1);
            }
            else if(target.x < (resX - 0.25*resX)) //If cube is too far forward
            {
                move_forward(1);
            }
#ifdef SHOW_COUT
            cout << "r: " << target.x << ", " << target.y;
#endif
        }
        ///If Green Cube is present and not red
        //Goes to right stack
        //Moves to have cube with center at bottom right corner
        else if(blockCenter_g.size() > 0)
        {
            cubeNotFound = 0;
            Point2f target = blockCenter_g[0];


            if((target.x > (resX - 0.25*resX))&&(target.y > (resY - 0.25*resY))&&(target.y < (resY - 0.1*resY))) //If cube is in corner
            {
                move_forward(1);
                cubeFound++;
            }
            else if(target.y < (resY - 0.25*resY)) //If cube is too far right
            {
                turn(1);
            }
            else if(target.y > (resY - 0.1*resY)) //If cube is too far left
            {
                turn(-1);
            }
            else if(target.x < (resX - 0.25*resX)) //If cube is too far forward
            {
                move_forward(1);
            }
#ifdef SHOW_COUT
            cout << "g: " << target.x << ", " << target.y;
#endif
        }
        else
        {
            cubeNotFound++;
#ifdef SHOW_COUT
            cout << cubeNotFound << endl;
#endif
            ///If no cube present (accounting for noise)
            if(stackNotFound == 20)
            {
                move_forward(-1);
            }
            else if(stackNotFound > 20)
            {
                turn(-1); //Turn
            }
        }
}

void Camera::moveTowardsStack() {
        Mat image;
        Mat imageDrawn;
        Mat src_bin_r;
        Mat src_bin_g;
	Point2f block;

        cap>>image;             //Store camera livestream to frame

        ColorThresholding(image, thresCanny, src_bin_r, src_bin_g);
        StackContours(image, src_bin_r, src_bin_g, imageDrawn, block);
	//#ifndef NDEBUG
        imshow("Camera Feed", imageDrawn);   //Display camera
	//#endif
        ///Actions to do if cube stack is found

        if((block.x != 0)&&(block.y != 0))
        {
	    stackNotFound = 0;
            ///Fix the decimal values
            if((block.x > (resX - 0.1*resX))&&(block.y > 0.4*resY)&&(block.y < 0.6*resY)) //If cube is in corner
            {
	      move_forward(1);
                stackFound = 1;
            }
            else if(block.y < 0.4*resY) //If cube is too far right
            {
                turn(1);
            }
            else if(block.y > 0.6*resY) //If cube is too far left
            {
                turn(-1);
            }
            else if(block.x < (resX - 0.1*resX)) //If cube is too far forward
            {
	        move_forward(0);
		waitKey(1000);
                move_forward(1);
		waitKey(1000);
            }
#ifdef SHOW_COUT
            cout << block.x << ", " << block.y << endl;
#endif
        }
        else
        {
            stackNotFound++;
#ifdef SHOW_COUT
            cout << stackNotFound << endl;
#endif
            ///If no cube stack present (accounting for noise)
            if(stackNotFound == 50)
            {
                move_forward(-1); //Move back
            }
            else if(stackNotFound > 20)
            {
                turn(-1); //Turn
            }
        }

}



