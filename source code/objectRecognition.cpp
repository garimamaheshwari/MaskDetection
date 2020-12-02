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
  /* Initialize dimension size for transformation space. */
  int xScale = dimensionSize(maxXScale, incrementScale);
  int yScale = dimensionSize(maxYScale, incrementScale);
  int rotationScale = dimensionSize(maxRotation, incrementRotation);
  int dims[] = {xScale, yScale, rotationScale};

  /* Create 3D histogram, which will be used to calculate scales and rotation. */
  Mat hist(3, dims, CV_32S, Scalar::all(0));

  for (int row = 0; row < xScale; ++row) {
    for (int col = 0; col < yScale; ++col) {
      for (int depth = 0; depth < rotationScale; ++depth) {
      
      } 
    }
  }

  /* Initialize transformation space. */
  scaleAndRotation = hist.clone();
}

/* Purpose: To calculate the dimension size of a given transformation axis.
 * Pre-conditions: none.
 * Post-conditions: Returns a number that corresponds to an axis' size for the
 *          transformation space. */
int ObjectRecognition::dimensionSize(double transform, int increment) const {
  double size = transform / increment;

  /* If the size is a whole number, return size. Otherwise, increment the size
   * by one and downcast to an int. This prevents a loss of data. */
  if (floor(size) == size)
    return size;

  return static_cast<int>(size) + 1;
}
