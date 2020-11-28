#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#include <map>
using namespace cv;
using namespace std;

void edgeDetection(Mat &image, string imageType, pair<int, int> k, pair<double, double> sigma,
                   pair<double, double> threshold) {
  //Convert image to grayscale
  cvtColor(image, image, COLOR_BGR2GRAY);

  // Perform Gaussian Blur on exemplar image
  Size kSize(k.first, k.second);
  GaussianBlur(image, image, kSize, sigma.first, sigma.second);

    // Display blurred image
  // namedWindow("Blurred Image");
  // imshow("Blurred Image", image);
  // waitKey(0);

  // Perform edge detection on blurred image
  Canny(image, image, threshold.first, threshold.second);

  // Display edge detection results
  namedWindow(imageType + ": Canny Edge Detection output");
  imshow(imageType  + ": Canny Edge Detection output", image);
  waitKey(0);
}

void readImage(Mat& input,string imageType) {

  // Display image
  namedWindow(imageType);
  imshow(imageType, input);
  waitKey(0);
	
  edgeDetection(input, imageType, make_pair(7,7), make_pair(2.0,2.0), make_pair(50,55));
}

