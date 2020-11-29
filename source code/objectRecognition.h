#pragma once
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#include <map>
using namespace cv;
using namespace std;

const int maxPixelValue = 750;
const int incrementScale = 0.05;
const int incrementRotation = 5;

class objectRecongition {

public:
  objectRecongition(const Mat& exemplar);
  void transformationSpace();
  bool match(Mat &searchImage);

private:
  Mat exemplar;
  // create 1 2D and 1 3D array:

  //// 1 2D array: translation in x and y
  //Mat translations;
  // 1 3D array: rotation and scale in x and y
  Mat scaleAndRotation;

  double maxXScale;
  double maxYScale;
};