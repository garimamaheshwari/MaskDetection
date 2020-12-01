/* Authors: Garima Maheshwari and Hailey Schauman
 * Date: 11/30/2020
 *
 * Description: A class that handles object recognition with an exemplar image
 * and a search image. Creates a transformation space that focuses on
 * translation, scaling, and rotation. Using this transformation space, the
 * exemplar image is tested against the search image. If it surpases a certain
 * threshold, a match exists. */
#pragma once
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
using namespace std;

/* VARIABLES USED FOR TRANSFORMATION SPACE. */

/* Variable for max size for an exemplar image.
 * Used to calculate the maximum size of scaling. */
const int maxPixelValue = 750;
/* Variable used for the increment when scaling an image. */
const int incrementScale = 0.05;
/* Variable used for the increment when rotating an image. */
const int incrementRotation = 5;

class ObjectRecognition {
public:
  /* Purpose: Constructor to create object and initialize data members.
   * Pre-conditions: Parameter is a valid image (e.g., not .gif).
   * Post-conditions: Initializes data members. */
  ObjectRecognition(const Mat &exemplar);
  /* Purpose: To perform object recognition on an exemplar and searchImage.
   * Pre-conditions: searchImage is a valid image (e.g., not .gif).
   * Post-conditions: Returns true if the exemplar is found in the image. */
  bool match(Mat &searchImage);

private:
  /* Purpose: To create a transformation space for the exemplar.
   * Pre-conditions: none.
   * Post-conditions: Creates a transformation space for scaling/rotation. */
  void transformationSpace();

  /* Stores the exemplar. */
  Mat exemplar;

  /* 3D matrix that stores rotation and xScale and yScale combinations.
   * Used for transformation space. */
  Mat scaleAndRotation;

  /* Stores the maximum size of scaling an image. */
  double maxXScale;
  double maxYScale;
};

/* THINGS NOT USED ANYMORE. */

/*
 *  // 1 2D array: translation in x and y
 *  Mat translations;
 */
