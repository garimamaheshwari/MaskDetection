/* Authors: Garima Maheshwari and Hailey Schauman
 * Date: 11/30/2020
 *
 * Description: A class that handles object recognition with an exemplar image
 * and a search image. Creates a transformation space that focuses on
 * translation, scaling, and rotation. Using this transformation space, the
 * exemplar image is tested against the search image. If it surpases a certain
 * threshold, a match exists. */
#pragma once
#include <iomanip>
#include <iostream>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

/* Edge value in an edge-detected image: */
const int edge = 255;

/* VARIABLES USED FOR TRANSFORMATION SPACE. */

/* Structure that stores the transformations in the scaling and rotation of a
 * transformation space: */
struct Transformations {
  double xScale;
  double yScale;
  int rotation;
};

class ObjectRecognition {
public:
  /* Purpose: Constructor to create object and initialize data members.
   * Pre-conditions: Parameter is a valid image (e.g., not .gif).
   * Post-conditions: Initializes data members. */
  ObjectRecognition(const Mat &exemplar);
  /* Purpose: Destructor to remove dynamic memory.
   * Pre-conditions: None.
   * Post-conditions: Deletes struct objects in transformCombinations. */
  ~ObjectRecognition();
  /* Purpose: To perform object recognition on an exemplar and searchImage.
   * Pre-conditions: searchImage is a valid image (e.g., not .gif).
   * Post-conditions: Returns true if the exemplar is found in the image. */
  bool match(Mat &searchImage);
  /* Purpose: To create a transformation space for the exemplar.
   * Pre-conditions: None.
   * Post-conditions: Creates a transformation space for scaling/rotation. */
  void transformationSpace();
  /* Purpose: To print transformation space;
   * Pre-conditions: Transformation space has been created.
   * Post-conditions: Returns the transformation space and its contents to the
   *          window. */
  void printTransformationSpace() const;

private:
  /* Purpose: Divide and conquer with translations.
   * Pre-conditions: None.
   * Post-conditions: Returns the highest count if there was a match. */
  int divideAndConquer(const Mat &searchImage,
                       pair<int, int> startingPoint, pair<int, int> dimensions, int currentCount,
                       int greatestCount) const;
  /* Purpose: Divide and conquer in the transformation space.
   * Pre-conditions: None.
   * Post-conditions: Greatest count of edge matches in the transformation space
   *          in that translation. */
  int divideAndConquerScale(const Mat &searchImage, pair<int, int> translation,
                            pair<int, int> startingPoint,
                            pair<int, int> dimensions, int currentCount, int previousCount, int levelOfDivide) const;
  /* Purpose: To calculate the dimension size of a given transformation axis.
   * Pre-conditions: None.
   * Post-conditions: Returns a number that corresponds to an axis' size for the
   *          transformation space. */
  int dimensionSize(double transform, double increment) const;
  /* Purpose: To calculate the count given the transformation.
   * Pre-conditions: None.
   * Post-conditions: Returns the count of the given point on the image.  */
  int getCount(const Mat &searchImage, pair<double, double> scale, int rotation,
               pair<int, int> origin) const;
  /* Purpose: To check the neighbors of a given (row, col) to see if edge.
   * Pre-conditions: None.
   * Post-conditions: Returns true if an edge exists.  */
  bool checkNeighbors(const Mat &searchImage, double row, double col) const;
  /* Purpose: To check the bound of a given transformed image.
   * Pre-conditions: None.
   * Post-conditions: Returns true if the image is within its bound.  */
  bool checkBounds(double xScale, double yScale, int numberOfEdges,
                   int levelOfDivide) const;

  /* Purpose: To convert a scale number of edges to scalex = 1 and scaley = 1.
   * Pre-conditions: xScale value, yScale value, and the number of edges
   * Post-conditions: Returns the number of edges scaled to 1 */
  int scaledEdges(double xScale, double yScale, int numberOfEdges) const;

  /* Stores the exemplar: */
  Mat exemplar;
  /* 3D vector that stores rotation and xScale and yScale combinations.
   * Used for transformation space: */
  vector<vector<vector<Transformations *>>> transformCombinations;
  /* Stores the maximum size of scaling an image: */
  double maxXScale;
  double maxYScale;

  /* Variable for max size for an exemplar image.
   * Used to calculate the maximum size of scaling: */
  const int maxPixelValue = 750;
  /* Variable used for the increment when scaling an image: */
  const double incrementScale = 0.10;
  /* Variables used for the increment when rotating an image: */
  const int incrementRotation = 60;
  const int maxRotation = 180;

  /* Number of edges in exemplar: */
  double exemplarEdges;
  const int edgeThreshold = 600;
  int bounds;
  

  /*Search image variables: */

  double searchEdges;
  double searchTotalPixels;

  /* Number of buckets: */
  const int bucketSize = 4;

  /*The transformations that must occur for the best counts stored*/
  map <int, pair<Transformations* , pair<int, int>>> bestTransformation;
};

/* THINGS NOT USED ANYMORE. */

/*
 *  // 1 2D array: translation in x and y
 *  Mat translations;
 */
