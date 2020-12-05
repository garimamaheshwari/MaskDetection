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

  /* Count number of edges in exemplar image: */
  exemplarEdges = 0;
  for (int row = 0; row < exemplar.rows; ++row) {
    for (int col = 0; col < exemplar.cols; ++col) {
      if (exemplar.at<uchar>(row, col) == edge) {
        exemplarEdges++;
      }
    }
  }

  bounds = exemplarEdges / 1.5;
}

/* Purpose: Destructor to remove dynamic memory.
 * Pre-conditions: None.
 * Post-conditions: Deletes Transformations structs in transformCombinations. */
ObjectRecognition::~ObjectRecognition() {
  /* Iterate through the 3D vector to remove dynamically allocated struct
   * objects: */
  for (int row = 0; row < transformCombinations.size(); ++row) {
    for (int col = 0; col < transformCombinations[row].size(); ++col) {
      for (int depth = 0; depth < transformCombinations[row][col].size();
           ++depth)
        delete transformCombinations[row][col][depth];
    }
  }
}

/* Purpose: To perform object recognition on an exemplar and searchImage.
 * Pre-conditions: searchImage is a valid image (e.g., not .gif) that has
 * already been cropped Post-conditions: Returns true if the exemplar is found
 * in the image. */
bool ObjectRecognition::match(Mat &searchImage) {
  /* Divide and Conquer variables: */
  int greatestCount = 0;

  /* Iterate through the number of translations: */
  for (int row = 0; row < searchImage.rows; row += 5) {
    for (int col = 0; col < searchImage.cols; col += 5) {

      /* int r = transformCombinations.size();
       int c = transformCombinations[0].size();
       int d = transformCombinations[0][0].size();
       int total = r * d * c; */

      /* Dive and conquer the transformation space at a specific translation: */
      pair<int, int> dimensions;
      dimensions.first = dimensionSize(maxXScale, incrementScale);
      dimensions.second = dimensionSize(maxYScale, incrementScale);
      greatestCount = divideAndConquer(searchImage, make_pair(row, col),
                                       make_pair(0, 0), dimensions, -1);

      /* Check to see if there was a match: */
      if (greatestCount != -1) {
        cout << "Exemplar Edges: " << exemplarEdges << endl;
        cout << "Count: " << greatestCount << endl;
        cout << "RETURNED TRUE." << endl;
        return true;
      }
    }
  }

  cout << "Exemplar Edges: " << exemplarEdges << endl;
  cout << "Count: " << greatestCount << endl;

  /* If it is within exemplar edge range, a match exists. Return true.
   * Otherwise, return false: */
  return false;
}

/* Purpose: Divide and conquer in the transformation space.
 * Pre-conditions: None.
 * Post-conditions: Greatest count of edge matches in the transformation space
 *          in that translation. */
int ObjectRecognition::divideAndConquer(const Mat &searchImage,
                                        pair<int, int> translation,
                                        pair<int, int> startingPoint,
                                        pair<int, int> dimensions,
                                        int greatestCount) const {
  /* Check if it meets the threshold: */
  if (greatestCount > exemplarEdges - edgeThreshold &&
      greatestCount < exemplarEdges + edgeThreshold) {
    return greatestCount;
  }

  /* Calculate new dimensions: */
  pair<int, int> newDimensions =
      make_pair(dimensions.first / 2, dimensions.second / 2);

  /* Check if there are no more cells to divide: */
  if (newDimensions.first == 0 || newDimensions.second == 0) {
    return -1;
  }

  int tRow = translation.first;
  int tCol = translation.second;
  pair<double, double> origin = make_pair(tRow, tCol);

  /* Keep track of the number of edge matches in each rectange: */
  map<pair<int, int>, int> edgeCounts;

  /* Divide and conquer: */
  for (int x = startingPoint.first; x < startingPoint.first + dimensions.first;
       x += dimensions.first / 2) {
    for (int y = startingPoint.second;
         y < startingPoint.second + dimensions.second;
         y += dimensions.second / 2) {

      int row = (x + dimensions.first) / 4;
      int col = (y + dimensions.second) / 4;

      int count = 0;
      for (int depth = 0; depth < transformCombinations[row][col].size();
           ++depth) {
        pair<double, double> scale =
            make_pair(transformCombinations[row][col][depth]->xScale,
                      transformCombinations[row][col][depth]->yScale);
        int depthCount =
            getCount(searchImage, scale,
                     transformCombinations[row][col][depth]->rotation, origin);

        if (depthCount > count) {
          count = depthCount;
        }
      }

      edgeCounts[make_pair(x, y)] = count;
    }
  }

  /* If the count is greater than the bounds, go into the given cell and divide
   * and conquer: */
  for (auto it = edgeCounts.begin(); it != edgeCounts.end(); ++it) {
    if (it->second > bounds) {
      pair<int, int> newStartingPoint =
          make_pair(it->first.first, it->first.second);
      greatestCount = divideAndConquer(searchImage, origin, newStartingPoint,
                                       newDimensions, it->second);
      if (greatestCount != -1) {
        return greatestCount;
      }
    }
  }
  return -1;
}

/* Purpose: To calculate the count given the transformation.
 * Pre-conditions: None.
 * Post-conditions: returns the count of the given point on the image  */
int ObjectRecognition::getCount(const Mat &searchImage,
                                pair<double, double> scale, int rotation,
                                pair<int, int> origin) const {
  int count = 0;
  /* Iterate through the exemplar: */
  for (int rowEx = 0; rowEx < exemplar.rows; ++rowEx) {
    for (int colEx = 0; colEx < exemplar.cols; ++colEx) {
      /* Transform exemplar point to match to search image: */
      if (exemplar.at<uchar>(rowEx, colEx) != edge) {
        continue;
      }

      /* Set the exemplar point with respect to origin: */
      double newRow =
          static_cast<double>(rowEx) + static_cast<double>(origin.first);
      double newCol =
          static_cast<double>(colEx) + static_cast<double>(origin.second);

      /* Perform the scale transformation: */
      newRow = static_cast<double>(newRow / scale.second);
      newCol = static_cast<double>(newCol / scale.first);

      /* Perform the rotation transformation and convert degrees to radians.
       * Then find cos and sin values: */
      const double pi = 3.14159265;
      double cosVal = cos(rotation * (pi / 180));
      double sinVal = sin(rotation * (pi / 180));

      double radianCol = (cosVal * newCol) + (sinVal * newRow);
      double radianRow = (-sinVal * newCol) + (cosVal * newRow);

      newCol = radianCol;
      newRow = radianRow;

      /* Check if edge: */
      if (checkNeighbors(searchImage, newRow, newCol)) {
        count++;
      }
    }
  }
  return count;
}

/* Purpose: To check the neighbors of a given (row, col) to see if edge.
 * Pre-conditions: None.
 * Post-conditions: Returns true if an edge exists.  */
bool ObjectRecognition::checkNeighbors(const Mat &searchImage, double row,
                                       double col) const {
  /* New row and cols: */
  int newRow = static_cast<int>(row);
  int newCol = static_cast<int>(col);

  /* Iterate through the neighbors: */
  for (int rowPlus = -1; rowPlus <= 1; ++rowPlus) {
    for (int colPlus = -1; colPlus <= 1; ++colPlus) {

      /* Update the new row and col: */
      int changeCol = newCol + colPlus;
      int changeRow = newRow + rowPlus;

      /* Check to see if the range is valid: */
      if (changeCol >= 0 && changeCol < searchImage.cols && changeRow >= 0 &&
          changeRow < searchImage.rows) {

        /* Check to see if there is an edge: */
        if (searchImage.at<uchar>(changeRow, changeCol) == edge)
          return true;
      }
    }
  }

  return false;
}

/* Purpose: To create a transformation space for the exemplar.
 * Pre-conditions: None.
 * Post-conditions: Creates a transformation space for scaling/rotation. */
void ObjectRecognition::transformationSpace() {
  /* Initialize dimension size for transformation space. */
  int xScale = dimensionSize(maxXScale, incrementScale);
  int yScale = dimensionSize(maxYScale, incrementScale);
  int rotationScale = dimensionSize(maxRotation, incrementRotation);

  /* Calculate the transformation combinations per (row, col, z): */
  double xIncrement = 0;
  double yIncrement = 0;
  int rotation = 0;

  /* Iterate through and calculate the xScale, yScale, and rotation
   * combinations: */
  for (int row = 0; row < xScale; ++row) {
    vector<vector<Transformations *>> transformSpace;
    for (int col = 0; col < yScale; ++col) {
      vector<Transformations *> temp;
      for (int depth = 0; depth < rotationScale; ++depth) {

        /* Create transformation combination for the transformation space: */
        Transformations *newTransformCombo = new Transformations;
        newTransformCombo->xScale = xIncrement;
        newTransformCombo->yScale = yIncrement;
        newTransformCombo->rotation = rotation;

        /* Increment rotation for next iteration: */
        rotation += incrementRotation;
        temp.push_back(newTransformCombo);
      }

      /* Increment yScale for next iteration: */
      yIncrement += incrementScale;
      rotation = 0;
      transformSpace.push_back(temp);
    }

    /* Increment xScale for next iteration: */
    xIncrement += incrementScale;
    yIncrement = 0;
    transformCombinations.push_back(transformSpace);
  }
}

/* Purpose: To print transformation space;
 * Pre-conditions: Transformation space has been created.
 * Post-conditions: Returns the transformation space and its contents to the
 *          window. */
void ObjectRecognition::printTransformationSpace() const {
  cout << "****TRANSFORMATION SPACE VALUES****" << endl;
  for (int row = 0; row < transformCombinations.size(); ++row) {
    for (int col = 0; col < transformCombinations[row].size(); ++col) {
      for (int depth = 0; depth < transformCombinations[row][col].size();
           ++depth) {
        cout << "ROW: " << row << " COL: " << col << " DEPTH: " << depth;
        cout << "\tRotation: "
             << transformCombinations[row][col][depth]->rotation;
        cout << "\txScale: " << transformCombinations[row][col][depth]->xScale;
        cout << "\tyScale: " << transformCombinations[row][col][depth]->yScale
             << endl;
      }
    }
  }
}

/* Purpose: To calculate the dimension size of a given transformation axis.
 * Pre-conditions: None.
 * Post-conditions: Returns a number that corresponds to an axis' size for the
 *          transformation space. */
int ObjectRecognition::dimensionSize(double transform, double increment) const {
  double size = transform / increment;

  /* If the size has a decimal greater than .5, increment the size by 1 to
   * keep important data when doing floor() conversion. Otherwise, return the
   * size that has been down casted to a whole number: */
  if (size - floor(size) > 0.5)
    return static_cast<int>(size) + 1;

  return static_cast<int>(size);
}
