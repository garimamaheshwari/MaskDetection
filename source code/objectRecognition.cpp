/* Authors: Garima Maheshwari and Hailey Schauman
 * Date: 11/30/2020
 *
 * Description: A class that handles object recognition with an exemplar image
 * and a search image. Creates a transformation space that focuses on
 * translation, scaling, and rotation. Using this transformation space, the
 * exemplar image is tested against the search image. If it surpases a certain
 * threshold, a match exists. */
#include "objectRecognition.h"

/* Purpose: Constructor to create object and initialize data members.
 * Pre-conditions: Parameter is a valid image (e.g., not .gif).
 * Post-conditions: Initializes data members. */
ObjectRecognition::ObjectRecognition(const Mat &exemplar) {
  this->exemplar = exemplar.clone();
  maxXScale = maxPixelValue / exemplar.rows;
  maxYScale = maxPixelValue / exemplar.cols;
}

/* Purpose: To perform object recognition on an exemplar and searchImage.
 * Pre-conditions: searchImage is a valid image (e.g., not .gif).
 * Post-conditions: Returns true if the exemplar is found in the image. */
bool ObjectRecognition::match(Mat &searchImage) { return false; }

/* Purpose: To create a transformation space for the exemplar.
 * Pre-conditions: none.
 * Post-conditions: Creates a transformation space for scaling/rotation. */
void ObjectRecognition::transformationSpace() {

  // translations = exemplar.clone();
  // creating 3D array
  //  int dims[] = {maxXScale / incrementScale, maxYScale / incrementScale,
  //              360 / incrementRotation};
  //// create 3D histogram of integers initialized to zero
  // Mat hist(3, dims, CV_32S, Scalar::all(0));
  //// for (int row = 0; row < foreground.rows; row++) {
  ////  for (int col = 0; col < foreground.cols; col++) {
  ////    // foreground.at<Vec3b>(r, c)[b]
  ////    int r = foreground.at<Vec3b>(row, col)[2] / bucketSize;
  ////    int g = foreground.at<Vec3b>(row, col)[1] / bucketSize;
  ////    int b = foreground.at<Vec3b>(row, col)[0] / bucketSize;

  ////    hist.at<int>(r, g, b) += 1;
  ////  }
  ////}
  ////

  // for (int row = 0; row < foreground.rows; row++) {
  //  for (int col = 0; col < foreground.cols; col++) {

  //  }
  //}
}
