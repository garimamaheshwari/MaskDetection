/* Authors: Garima Maheshwari and Hailey Schauman
 * Date: 11/30/2020
 *
 * Description: Helper functions for reading in images and performing edge
 * detection, so that the exemplar and search images are ready for object
 * detection. */
#include "objectRecognition.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

/* Threshold deviation to automatically detect minimum and maximum threshold in
 * edge detection. */
const int thresholdDev = 25;
/* Kernel size for blurring. */
const pair<int, int> kernel(7, 7);

/* Purpose: To perform edge detection on an image.
 * Pre-conditions: Parameters are valid.
 * Post-conditions: Changes the color image to an edge-detected image and
 *            outputs this image. */
void edgeDetection(Mat &image, string imageType,
                   pair<double, double> sigma) {
  /* Convert image to gray-scale and then perform Gaussian blur on the image. */
  cvtColor(image, image, COLOR_BGR2GRAY);
  Size kSize(kernel.first, kernel.second);
  GaussianBlur(image, image, kSize, sigma.first, sigma.second);

  /* Obtain edge detection thresholds. */
  double average = mean(image)[0];
  pair<double, double> threshold;
  threshold.first = abs(average - thresholdDev);
  threshold.second = abs(average + thresholdDev);

  /* Perform edge detection on the image. */
  Canny(image, image, threshold.first, threshold.second);

  /* Display the edge-detected image to the output window. */
  namedWindow(imageType + ": Canny Edge Detection output");
  imshow(imageType + ": Canny Edge Detection output", image);
  waitKey(0);
}

/* Purpose: Read in an image and perform edge detection on it.
 * Pre-conditions: image is valid (e.g., not .gif).
 * Post-conditions: Transforms image by having it undergo edge detection. */
void readImage(Mat &image, string imageType) {
  /* Resize image if needed. */


  /* Display original image for comparison. */
  namedWindow(imageType);
  imshow(imageType, image);
  waitKey(0);

  /* Perform edge detection on image. */
  edgeDetection(image, imageType, make_pair(2.0, 2.0));
}

/* THINGS NOT IN USE. */

/* In edge detection function. */

// Display blurred image
// namedWindow("Blurred Image");
// imshow("Blurred Image", image);
// waitKey(0);

// Mat scaled;
////checking scaling
// resize(image, scaled, scaled.size(), 2.5, 2.5);

//// Display edge detection results
// namedWindow(imageType + ": Canny Edge Detection output");
// imshow(imageType + ": Canny Edge Detection output", scaled);
// waitKey(0);
