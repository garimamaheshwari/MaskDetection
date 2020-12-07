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
  if (maxXScale > 2.0) {
    maxXScale = 2.0;
  }
  maxYScale = maxPixelValue / exemplar.cols;
  if (maxYScale > 2.0) {
    maxYScale = 2.0;
  }

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
  /* Divide and conquer the translation of an image: */
  //pair<int, int> dimensions = make_pair(searchImage.rows, searchImage.cols);
  //int greatestCount =
  //    divideAndConquer(searchImage, make_pair(0, 0), dimensions, -1, -1);

  ///* Check to see if there was a match. If there is, return true. Otherwise,
  // * return false: */
  //if (greatestCount != -1) {
  //  cout << "Exemplar Edges: " << exemplarEdges << endl;
  //  cout << "Count: " << greatestCount << endl;
  //  cout << "RETURNED TRUE." << endl;
  //  return true;
  //}

  //cout << "Exemplar Edges: " << exemplarEdges << endl;
  //cout << "Count: " << greatestCount << endl;

  //return false;


    /* total number of pixels within a search image */
  searchTotalPixels = searchImage.rows * searchImage.cols;

    /* count number of edges within the searchImage */
  searchEdges = 0;
  for (int row = 0; row < searchImage.rows; ++row) {
    for (int col = 0; col < searchImage.cols; ++col) {
      if (searchImage.at<uchar>(row, col) == edge) {
        searchEdges++;
      }
    }
  }

    int greatestCount = 0;
  for (int row = 0; row < searchImage.rows; row += 100) {
      for (int col = 0;col < searchImage.cols; col += 100) {

      /* int r = transformCombinations.size();
       int c = transformCombinations[0].size();
       int d = transformCombinations[0][0].size();
       int total = r * d * c; */

      /* Dive and conquer the transformation space at a specific translation:
      */ pair<int, int> dimensions; 
      dimensions.first = dimensionSize(maxXScale, incrementScale); 
      dimensions.second =dimensionSize(maxYScale, incrementScale); 
      
      int count = divideAndConquerScale(searchImage, make_pair(row, col),
                                            make_pair(0, 0), dimensions, -1, -1, 1);

      /* Check to see if there was a match: */
      if (greatestCount < count) {
        greatestCount = count; 

      }
    }
  }
     cout << "Exemplar Edges: " << exemplarEdges << endl;
     cout << "Count: " << greatestCount << endl;
     cout << "RETURNED TRUE." << endl;
     return greatestCount != -1;
     
}

/* Purpose: Divide and conquer with translations.
 * Pre-conditions: None.
 * Post-conditions: Returns the highest count if there was a match. */
int ObjectRecognition::divideAndConquer(const Mat &searchImage,
                                        pair<int, int> startingPoint,
                                        pair<int, int> dimensions,
                                        int currentCount, int greatestCount) const {
  /* Check if it meets the threshold: */
  if (greatestCount > exemplarEdges - edgeThreshold &&
      greatestCount < exemplarEdges + edgeThreshold) {
    return greatestCount;
  }

  if (currentCount > greatestCount) {
    greatestCount = currentCount;  
  }

  /* Divide the dimensions by 2 to get 4 rectangles for divide and conquer: */
  pair<int, int> newDimensions =
      make_pair(dimensions.first / 2, dimensions.second / 2);

  /* Check to make sure there are no more cells to divide into. This means the
   * cell is too small to continue: */
  if (newDimensions.first == 0 || newDimensions.second == 0) {
    return -1;
  }

  /* Store the edge count from each respected quadrant: */
  map<pair<int, int>, int> edgeCounts;

  /* x and y increment: */
  int xIncrement = dimensions.first / bucketSize;
  int yIncrement = dimensions.second / bucketSize;

  /* Go into the middle of each quadrant and use the (x, y) at that location for
   * the transformation space: */
  for (int x = 1; x < 3; ++x) {
    /* Re-initialize the y-increment for its next iteration: */
    yIncrement = dimensions.second / bucketSize;
    /* Calculate the x value that is in the middle of a quadrant: */
    int row = xIncrement + startingPoint.first;

    for (int y = 1; y < 3; ++y) {
      /* Calculate the y value that is in the middle of a quadrant: */
      int col = yIncrement + startingPoint.second;

      /* Divide and conquer the transformation space at a translation: */
      pair<int, int> dimensionsScale;
      dimensionsScale.first = dimensionSize(maxXScale, incrementScale);
      dimensionsScale.second = dimensionSize(maxYScale, incrementScale);
      int count = divideAndConquerScale(searchImage, make_pair(row, col),
                                        make_pair(0, 0), dimensionsScale, -1, -1, 1);

      /* Check to see if there was a match: */
      if (count != -1) {
        edgeCounts[make_pair(row, col)] = count;
      }

      /* Increment y to the next quadrant in the middle: */
      yIncrement += (y + 1) * yIncrement;
    }

    /* Increment x to the next quadrant in the middle: */
    xIncrement += (x + 1) * xIncrement;
  }

  /* If the count is greater than the bounds, go into the given cell and divide
   * and conquer: */
  for (auto it = edgeCounts.begin(); it != edgeCounts.end(); ++it) {
      /* Have new origin be upper right hand of the matched quadrant: */
      int newRow = it->first.first - (dimensions.first / bucketSize);
      int newCol = it->first.second - (dimensions.second / bucketSize);
      pair<int, int> newPoint = make_pair(newRow, newCol);
      currentCount = divideAndConquer(searchImage, newPoint,
                                       newDimensions, it->second, greatestCount);
    
  }

  return max(currentCount, greatestCount);
}

/* Purpose: Divide and conquer in the transformation space.
 * Pre-conditions: None.
 * Post-conditions: Greatest count of edge matches in the transformation space
 *          in that translation. */
int ObjectRecognition::divideAndConquerScale(const Mat &searchImage,
                                             pair<int, int> translation,
                                             pair<int, int> startingPoint,
                                             pair<int, int> dimensions,
                                             int currentCount, int previousCount, int levelOfDivide) const {
 ///* ///* Check if it meets the threshold: */
 // if (greatestCount > exemplarEdges - edgeThreshold &&
 //     greatestCount < exemplarEdges + edgeThreshold) {
 //   return greatestCount;
 // }*/

   if (currentCount < previousCount) {
    return previousCount;
  }

   previousCount = currentCount;

  /* Divide the dimensions by 2 to get 4 rectangles for divide and conquer: */
  pair<int, int> newDimensions =
      make_pair(dimensions.first / 2, dimensions.second / 2);

  /* Check to make sure there are no more cells to divide into. This means the
   * cell is too small to continue: */
  if (newDimensions.first == 0 || newDimensions.second == 0) {
    return previousCount;
  }

  /* Obtain the origin of where the exemplar will be placed: */
  int tRow = translation.first;
  int tCol = translation.second;
  pair<double, double> origin = make_pair(tRow, tCol);

  /* Store the edge count from each respected quadrant: */
  map<pair<int, int>, int> edgeCounts;

  /* x and y increment: */
  int xIncrement = dimensions.first / bucketSize;
  int yIncrement = dimensions.second / bucketSize;
  pair<double, double> scale = make_pair(0, 0);

  int bestRotation;
  /* Divide and conquer: */
  for (int x = 1; x < 3; ++x) {
    /* Re-initialize the y-increment for its next iteration: */
    yIncrement = dimensions.second / bucketSize;
    /* Calculate the x value that is in the middle of a quadrant: */
    int row = xIncrement + startingPoint.first;

    for (int y = 1; y < 3; ++y) {
      /* Calculate the y value that is in the middle of a quadrant: */
      int col = yIncrement + startingPoint.second;

      /* Keep track of the highest count at a given scale transformation: */
      int count = 0;
      scale.first = transformCombinations[row][col][0]->xScale;
      scale.second = transformCombinations[row][col][0]->yScale;

      /* Iterate through multiple rotations, finding the best match: */
      for (int depth = 0; depth < transformCombinations[row][col].size();
           ++depth) {
        int depthCount =
            getCount(searchImage, scale,
                     transformCombinations[row][col][depth]->rotation, origin);

        if (depthCount > count) {
          bestRotation = depth;
          count = depthCount;
        }
      }

      /* Check to see if the count is within bounds: */
      if (checkBounds(scale.first, scale.second, count, levelOfDivide)) {
        /*Store bestCount in map with Score: */
        /*Transformations *newT = new Transformations();*/
        //newT->rotation = bestRotation;
        //newT->xScale = scale.first;
        //newT->yScale = scale.second;
        //bestTransformation[count] = make_pair(newT, origin));
        edgeCounts[make_pair(row, col)] = scaledEdges(scale.first,scale.second, count);
      }

      /* Increment y to the next quadrant in the middle: */
      yIncrement += (y + 1) * yIncrement;
    }

    /* Increment x to the next quadrant in the middle: */
    xIncrement += (x + 1) * xIncrement;
  }
   
  int maxCount = previousCount;
  /* If the count is greater than the bounds, go into the given cell and
   * divide and conquer: */
  for (auto it = edgeCounts.begin(); it != edgeCounts.end(); ++it) {

    /* Have new origin be upper right hand of the matched quadrant: */
    int newRow = it->first.first - (dimensions.first / bucketSize);
    int newCol = it->first.second - (dimensions.second / bucketSize);
    pair<int, int> newPoint = make_pair(newRow, newCol);

    /* Dive and conquer on that new origin: */
    currentCount = divideAndConquerScale(searchImage, origin, newPoint, newDimensions,
                              it->second, previousCount, levelOfDivide + 1);
    maxCount = max(currentCount, maxCount);
  }
  return maxCount;
 

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

/* Purpose: To check the bound of a given transformed image.
 * Pre-conditions: None.
 * Post-conditions: Returns true if the image is within its bound.  */
bool ObjectRecognition::checkBounds(double xScale, double yScale,
                                    int numberOfEdges, int levelOfDivide) const {

  double scaleFactor = 0;
  if (xScale == yScale) {
    scaleFactor = xScale;
  } else {
    scaleFactor = sqrt(pow(xScale, 2) + pow(yScale, 2));
  }
  double newEdgeCount = exemplarEdges * scaleFactor;
  double ratioSearch = searchEdges / searchTotalPixels;
  double ratioExemplar = exemplarEdges / (exemplar.rows * exemplar.cols);
  double difference = (ratioSearch - ratioExemplar);
  double boundDecimal = 0.9;
  //if (difference < 0) {
  //  boundDecimal = 0.01;
  //}else if (difference > 0.06) {
  //  boundDecimal = 0.9;
  //} else {
  //  boundDecimal = 0.01;
  //}
  double percentage = 1 - (levelOfDivide * boundDecimal);
  double edgeBound = newEdgeCount * percentage;

  return (numberOfEdges > abs(exemplarEdges - edgeBound) &&
          numberOfEdges < exemplarEdges + edgeBound);
}

/* Purpose: To convert a scale number of edges to scalex = 1 and scaley = 1.
 * Pre-conditions: xScale value, yScale value, and the number of edges
 * Post-conditions: Returns the number of edges scaled to 1 */
int ObjectRecognition::scaledEdges(double xScale, double yScale,
                                   int numberOfEdges) const {
  double scaleFactor = 0;
  if (xScale == yScale) {
    scaleFactor = xScale;
  } else {
    scaleFactor = sqrt(pow(xScale, 2) + pow(yScale, 2));
  }
  return numberOfEdges / scaleFactor;
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
  double xIncrement = 0.5;
  double yIncrement = 0.5;
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
    yIncrement = 0.5;
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

/* MAY NEED LATER */

///* Iterate through the number of translations: */
// for (int row = 0; row < searchImage.rows; row += 5) {
//  for (int col = 0; col < searchImage.cols; col += 5) {

//    /* int r = transformCombinations.size();
//     int c = transformCombinations[0].size();
//     int d = transformCombinations[0][0].size();
//     int total = r * d * c; */

//    /* Dive and conquer the transformation space at a specific translation:
//    */ pair<int, int> dimensions; dimensions.first =
//    dimensionSize(maxXScale, incrementScale); dimensions.second =
//    dimensionSize(maxYScale, incrementScale); greatestCount =
//    divideAndConquerScale(searchImage, make_pair(row, col),
//                                          make_pair(0, 0), dimensions, -1);

//    /* Check to see if there was a match: */
//    if (greatestCount != -1) {
//      cout << "Exemplar Edges: " << exemplarEdges << endl;
//      cout << "Count: " << greatestCount << endl;
//      cout << "RETURNED TRUE." << endl;
//      return true;
//    }
//  }
//}