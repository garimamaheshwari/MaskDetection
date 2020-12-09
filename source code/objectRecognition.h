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

/* Keeps track of the best transformation: */
// static map<double, pair<Transformations, pair<int, int>>> bestTransformation;

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

  /* FUNCTIONS USED FOR THE TRANSFORMATION SPACE */

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
  /* FUNCTIONS USED FOR DIVIDE AND CONQUER */

  /* Purpose: Divide and conquer with translations.
   * Pre-conditions: All parameters are valid.
   * Post-conditions: Returns the highest count if there was a match. */
  double divideAndConquer(const Mat &searchImage, pair<int, int> startingPoint,
                          pair<int, int> dimensions, double currentCount,
                          double previousCount, int levelOfDivide) const;
  /* Purpose: Divide and conquer in the transformation space.
   * Pre-conditions: None.
   * Post-conditions: Greatest count of edge matches in the transformation space
   *          in that translation. */
  double divideAndConquerScale(const Mat &searchImage,
                               pair<int, int> translation,
                               pair<int, int> startingPoint,
                               pair<int, int> dimensions, double currentCount,
                               double previousCount, int levelOfDivide) const;

  /* FUNCTION USED FOR BOUNDS CHECKING */

  /* Purpose: To check the bound of a given transformed image.
   * Pre-conditions: None.
   * Post-conditions: Returns true if the ratio is within its bound.  */
  bool checkBounds(double ratio, int levelOfDivide) const;

  /* FUNCTIONS USED FOR FINDING MATCH EDGES */

  /* Purpose: To calculate the count given the transformation.
   * Pre-conditions: None.
   * Post-conditions: returns the total edges of an transformed exemplar and the
   *          total edge matches at a given point on the image  */
  pair<double, double> getCount(const Mat &searchImage,
                                pair<double, double> scale, int rotation,
                                pair<int, int> origin) const;
  /* Purpose: To check the neighbors of a given (row, col) to see if edge.
   * Pre-conditions: None.
   * Post-conditions: Returns true if an edge exists.  */
  bool checkNeighbors(const Mat &searchImage, double row, double col) const;

  /* HELPER FUNCTIONS */

  /* Purpose: Compute the total amount of edges in an image.
   * Pre-conditions: image is valid.
   * Post-conditions: Returns the number of edges in the image. */
  int computeEdgeTotals(const Mat &image) const;
  /* Purpose: Draw an outline of the match on the searchImage.
   * Pre-conditions: ratio has been computed and it is a good match.
   * Post-conditions: Outputs the image with an outline of the match. */
 // void drawMatch(const Mat &image, double ratio) const;
  /* Purpose: To calculate the dimension size of a given transformation axis.
   * Pre-conditions: None.
   * Post-conditions: Returns a number that corresponds to an axis' size for the
   *          transformation space. */
  int dimensionSize(double transform, double increment) const;

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
  /*Search image variables: */
  double searchEdges;
  double searchSize;
  /* Number of buckets: */
  const int bucketSize = 4;
  /* Max size of a scale: */
  const int maxScale = 2;
};