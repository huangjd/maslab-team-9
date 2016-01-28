#ifndef __CV_H__
#define __CV_H__

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>

#include "HAL.h"

void ColorThresholding(const cv::Mat& src, int thresVal, cv::Mat& thresOut_r, cv::Mat& thresOut_g);

void CubeContours(const cv::Mat& src, const cv::Mat& src_binary_r, const cv::Mat& src_binary_g, cv::Mat& src_contour, std::vector<cv::Point2f> &blockCenter_r, std::vector<cv::Point2f> &blockCenter_g);

void StackContours(const cv::Mat& src, const cv::Mat& src_binary_r, const cv::Mat& src_binary_g, cv::Mat& src_contour, cv::Point2f &block);

class Camera {
  const int resX;
  const int resY;
  cv::VideoCapture cap;

  const int thresCanny;
  int cubeNotFound;
  int cubeFound;
  int stackNotFound;
  int stackFound;

public:
  Camera();
  ~Camera();

  void moveTowardsCube();

  void moveTowardsStack();
};

#endif // __CV_H__
