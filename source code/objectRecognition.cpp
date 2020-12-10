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
  /* Store exemplar into data member: */
  this->exemplar = exemplar.clone();

  /* Calculate the maximum size of the xScale: */
  maxXScale = maxPixelValue / exemplar.rows;
  /* If xScale exceeds 2.0, set it to 2.00: */
  if (maxXScale > maxScale) {
    maxXScale = maxScale;
  }

  /* Calculate the maximum size of the yScale: */
  maxYScale = maxPixelValue / exemplar.cols;
  /* If yScale exceeds 2.0, set it to 2.00: */
  if (maxYScale > maxScale) {
    maxYScale = maxScale;
  }

  /* Count number of edges in exemplar image: */
  exemplarEdges = computeEdgeTotals(exemplar);

  /* Initialize other data members: */
  splitBy = 0;
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

/* FUNCTIONS USED FOR OBJECT RECONGITION / DIVIDE AND CONQUER */

/* Purpose: To perform object recognition on an exemplar and searchImage.
 * Pre-conditions: searchImage is a valid image (e.g., not .gif) that has
 * already been cropped.
 * Post-conditions: Returns true if the exemplar is found in the image. */
bool ObjectRecognition::match(Mat &searchImage, const Mat &original, string name) {
  /* Calculate the edges in the searchImage and the size of the searchImage to
   * get ratio: */
  searchEdges = computeEdgeTotals(searchImage);
  searchSize = static_cast<double>(searchImage.rows) *
               static_cast<double>(searchImage.cols);
  double searchImageRatio = searchEdges / searchSize;

  double greatestRatio = 0;

  /* If the image ratio of edges compared to pixels is high, use divide and
   * conquer on translation: */
  if (searchImageRatio > 0.05) {
    /* Calculate dimensions of the search image: */
    pair<int, int> dimensions = make_pair(searchImage.rows, searchImage.cols);

    /* Divide and conquer the translation of an image: */
    greatestRatio =
        divideAndConquer(searchImage, make_pair(0, 0), dimensions, -1, -1, 1);

  } else {
    /* Calculate dimensions for the transformation space: */
    pair<int, int> dimensions = make_pair(transformCombinations.size(),
                                           transformCombinations[0].size());
    double currentRatio = 0;

    /* Iterate through the image to receive translation values for divide and
     * conquer on scale: */
    for (int r = 0; r < searchImage.rows; r += 25) {
      for (int c = 0; c < searchImage.cols; c += 25) {
        currentRatio =
            divideAndConquerScale(searchImage, make_pair(r, c), make_pair(0, 0),
                                  dimensions, -1, -1, 1);
        if (currentRatio > greatestRatio) {
          greatestRatio = currentRatio;
        }
      }
    }
  }

  /* Check to see if there was a match. If there is, return true. Otherwise,
   * return false: */
  if (greatestRatio > 0.70) {
    cout << "RESULTS FOR " + name + ": " << endl;
    cout << "Person is wearing a mask. " << endl;
    cout << "Match Percentage: " << greatestRatio * 100 << "%" << endl;

    /* Illustrate the match and save to the default directory: */
    drawMatch(original, greatestRatio, name);

    return true;
  }

  cout << "RESULTS FOR " + name + ": " << endl;
  cout << "Person is not wearing a mask. " << endl;
  cout << "Match Percentage: " << greatestRatio * 100 << "%" << endl;

  /* Illustrate the non-match and save to the default directory: */
  drawMatch(original, greatestRatio, name);
  return false;
}

/* Purpose: Divide and conquer with translations.
 * Pre-conditions: All parameters are valid.
 * Post-conditions: Returns the highest count if there was a match. */
double ObjectRecognition::divideAndConquer(const Mat &searchImage,
                                           pair<int, int> startingPoint,
                                           pair<int, int> dimensions,
                                           double currentCount,
                                           double previousCount,
                                           int levelOfDivide) const {
  if (currentCount < previousCount) {
    return previousCount;
  }

  previousCount = currentCount;

  /* Divide the dimensions by 2 to get 4 rectangles for divide and conquer:
   */
  pair<int, int> newDimensions =
      make_pair(dimensions.first / 2, dimensions.second / 2);

  /* Check to make sure there are no more cells to divide into. This means
   * the cell is too small to continue: */
  if (newDimensions.first == 0 || newDimensions.second == 0) {
    return previousCount;
  }

  /* Store the edge count from each respected quadrant: */
  map<pair<int, int>, double> edgeCounts;

  /* x and y increment: */
  int xIncrement = dimensions.first / bucketSize;
  int yIncrement = dimensions.second / bucketSize;
  pair<double, double> scale = make_pair(0, 0);

  /* Create dimensions for transformation space: */
  pair<int, int> tSpaceDimensions;
  tSpaceDimensions.first = transformCombinations.size();
  tSpaceDimensions.second = transformCombinations[0].size();

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

      double result =
          divideAndConquerScale(searchImage, make_pair(row, col),
                                make_pair(0, 0), tSpaceDimensions, -1, -1, 1);

      /* Check to see if the count is within bounds: */
      if (checkBounds(result, levelOfDivide)) {
        edgeCounts[make_pair(row, col)] = result;
      }

      /* Increment y to the next quadrant in the middle: */
      yIncrement += (y + 1) * yIncrement;
    }

    /* Increment x to the next quadrant in the middle: */
    xIncrement += (x + 1) * xIncrement;
  }

  double maxCount = previousCount;
  /* If the count is greater than the bounds, go into the given cell and
   * divide and conquer: */
  for (auto it = edgeCounts.begin(); it != edgeCounts.end(); ++it) {

    /* Have new origin be upper right hand of the matched quadrant: */
    int newRow = it->first.first - (dimensions.first / bucketSize);
    int newCol = it->first.second - (dimensions.second / bucketSize);
    pair<int, int> newPoint = make_pair(newRow, newCol);

    /* Dive and conquer on that new origin: */
    currentCount =
        divideAndConquer(searchImage, newPoint, newDimensions, it->second,
                         previousCount, levelOfDivide + 1);

    maxCount = max(currentCount, maxCount);
  }

  return maxCount;
}

/* Purpose: Divide and conquer in the transformation space.
 * Pre-conditions: None.
 * Post-conditions: Greatest count of edge matches in the transformation
 * space in that translation. */
double ObjectRecognition::divideAndConquerScale(
    const Mat &searchImage, pair<int, int> translation,
    pair<int, int> startingPoint, pair<int, int> dimensions,
    double currentCount, double previousCount, int levelOfDivide) const {

  if (currentCount < previousCount) {
    return previousCount;
  }

  previousCount = currentCount;

  /* Divide the dimensions to get 4 rectangles for divide and conquer: */
  pair<int, int> newDimensions =
      make_pair(dimensions.first / 2, dimensions.second / 2);

  /* Check to make sure there are no more cells to divide into. This means
   * the cell is too small to continue: */
  if (newDimensions.first == 0 || newDimensions.second == 0) {
    return previousCount;
  }

  /* Obtain the origin of where the exemplar will be placed: */
  int tRow = translation.first;
  int tCol = translation.second;
  pair<double, double> origin = make_pair(tRow, tCol);

  /* Store the edge count from each respected quadrant: */
  map<pair<int, int>, double> edgeCounts;

  /* x and y increment: */
  int xIncrement = dimensions.first / bucketSize;
  int yIncrement = dimensions.second / bucketSize;
  pair<double, double> scale = make_pair(0, 0);

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
      double highestRatio = 0;
      scale.first = transformCombinations[row][col][0]->xScale;
      scale.second = transformCombinations[row][col][0]->yScale;

      /* Iterate through multiple rotations, finding the best match: */
      int rotation = 0;
      for (int depth = 0; depth < transformCombinations[row][col].size();
           ++depth) {
        pair<double, double> result =
            getCount(searchImage, scale,
                     transformCombinations[row][col][depth]->rotation, origin);
        double ratio = result.second / result.first;

        if (ratio > highestRatio) {
          highestRatio = ratio;
          rotation = transformCombinations[row][col][depth]->rotation;
        }
      }

      /* Check to see if the count is within bounds: */
      if (checkBounds(highestRatio, levelOfDivide)) {
        edgeCounts[make_pair(row, col)] = highestRatio;

        /* Create transformation object: */
        Transformations currentCombo;
        currentCombo.xScale = scale.first;
        currentCombo.yScale = scale.second;
        currentCombo.rotation = rotation;

        bestTransformation[highestRatio] = make_pair(currentCombo, origin);
      }

      /* Increment y to the next quadrant in the middle: */
      yIncrement += (y + 1) * yIncrement;
    }

    /* Increment x to the next quadrant in the middle: */
    xIncrement += (x + 1) * xIncrement;
  }

  double maxCount = previousCount;
  /* If the count is greater than the bounds, go into the given cell and
   * divide and conquer: */
  for (auto it = edgeCounts.begin(); it != edgeCounts.end(); ++it) {

    /* Have new origin be upper right hand of the matched quadrant: */
    int newRow = it->first.first - (dimensions.first / bucketSize);
    int newCol = it->first.second - (dimensions.second / bucketSize);
    pair<int, int> newPoint = make_pair(newRow, newCol);

    /* Dive and conquer on that new origin: */
    currentCount =
        divideAndConquerScale(searchImage, origin, newPoint, newDimensions,
                              it->second, previousCount, levelOfDivide + 1);
    maxCount = max(currentCount, maxCount);
  }
  return maxCount;
}

/* FUNCTION USED FOR BOUND CHECKING */

/* Purpose: To check the bound of a given transformed image.
 * Pre-conditions: None.
 * Post-conditions: Returns true if the ratio is within its bound.  */
bool ObjectRecognition::checkBounds(double ratio, int levelOfDivide) const {
  /* Get the amount of edges per the size of the search image to get ratio: */
  double searchImageRatio = searchEdges / searchSize;

  /* If the search image has a lot of edges compared to its size, it must pass a
   * bigger decimal to go further into divide and conquer: */
  if (searchImageRatio > 0.06) {
    if (ratio > 0.20 * (levelOfDivide))
      return true;

    return false;
  }

  /* If the search image does not have a lot of edges compared to its size, the
   * bound to be passed is smaller: */
  if (ratio > 0.15 * (levelOfDivide))
    return true;

  return false;
}

/* FUNCTIONS USED FOR FINDING MATCH EDGES */

/* Purpose: To calculate the count given the transformation.
 * Pre-conditions: None.
 * Post-conditions: returns the total edges of an transformed exemplar and
 * the total edge matches at a given point on the image  */
pair<double, double> ObjectRecognition::getCount(const Mat &searchImage,
                                                 pair<double, double> scale,
                                                 int rotation,
                                                 pair<int, int> origin) const {
  int count = 0;
  int totalEdges = 0;

  /* Iterate through the exemplar: */
  for (int rowEx = 0; rowEx < exemplar.rows; ++rowEx) {
    for (int colEx = 0; colEx < exemplar.cols; ++colEx) {
      /* Transform exemplar point to match to search image: */
      if (exemplar.at<uchar>(rowEx, colEx) != edge) {
        continue;
      }
      totalEdges++;

      double newRow = rowEx;
      double newCol = colEx;

      /* Perform the rotation transformation and convert degrees to radians.
       * Then find cos and sin values: */
      const double pi = 3.14159265;
      double cosVal = cos(rotation * (pi / 180));
      double sinVal = sin(rotation * (pi / 180));

      double radianRow = (sinVal * colEx) + (cosVal * rowEx);
      double radianCol = (cosVal * colEx) + (-sinVal * rowEx);

      newRow = radianRow;
      newCol = radianCol;

      /* Perform the scale transformation: */
      newRow = static_cast<double>(newRow * scale.second);
      newCol = static_cast<double>(newCol * scale.first);

      /* Set the exemplar point with respect to origin: */
      newRow += static_cast<double>(origin.first);
      newCol += static_cast<double>(origin.second);

      /* Check if edge: */
      if (checkNeighbors(searchImage, newRow, newCol)) {
        count++;
      }
    }
  }
  return make_pair(totalEdges, count);
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

/* FUNCTIONS USED FOR THE TRANSFORMATION SPACE */

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
 * Post-conditions: Returns a number that corresponds to an axis' size for
 * the transformation space. */
int ObjectRecognition::dimensionSize(double transform, double increment) const {
  double size = transform / increment;

  /* If the size has a decimal greater than .5, increment the size by 1 to
   * keep important data when doing floor() conversion. Otherwise, return the
   * size that has been down casted to a whole number: */
  if (size - floor(size) > 0.5)
    return static_cast<int>(size) + 1;

  return static_cast<int>(size);
}

/* HELPER FUNCTIONS */

/* Purpose: Compute the total amount of edges in an image.
 * Pre-conditions: image is valid.
 * Post-conditions: Returns the number of edges in the image. */
int ObjectRecognition::computeEdgeTotals(const Mat &image) const {
  int edgeSum = 0;
  /* Iterate through the image and keep a track of the number of edges
   * encountered: */
  for (int row = 0; row < image.rows; ++row) {
    for (int col = 0; col < image.cols; ++col) {
      if (image.at<uchar>(row, col) == edge) {
        edgeSum++;
      }
    }
  }

  return edgeSum;
}

/* Purpose: Draw an outline of the match on the searchImage.
 * Pre-conditions: ratio has been computed and it is a good match.
 * Post-conditions: Outputs the image with an outline of the match. */
void ObjectRecognition::drawMatch(const Mat &image, double ratio, string name) const {
  /* Create a new image and clone the original: */
  Mat newImage = image.clone();

  /* Get transformation from the best ratio: */
  pair<Transformations, pair<int, int>> transformation =
      bestTransformation[ratio];

  /* Get the dimensions of the box: */
  int row = transformation.first.yScale * exemplar.rows;
  int col = transformation.first.xScale * exemplar.cols;

  /* Get the starting point of the box: */
  int boxRow = transformation.second.first;
  int boxCol = transformation.second.second;

  /* Get row ranges: */
  int endR = row + boxRow;
  if (endR > newImage.rows) {
    endR = newImage.rows - 1;
  }

  /* Get col ranges: */
  int endC = col + boxCol;
  if (endC > newImage.cols) {
    endC = newImage.cols - 1;
  }

  /* Draw line segments: */
  line(newImage, Point(boxCol, boxRow), Point(boxCol, endR), (0, 255, 0), 3);
  line(newImage, Point(boxCol, endR), Point(endC, endR), (0, 255, 0), 3);
  line(newImage, Point(endC, endR), Point(endC, boxRow), (0, 255, 0), 3);
  line(newImage, Point(endC, boxRow), Point(boxCol, boxRow), (0, 255, 0), 3);

  /* Output to the window and save in the directory folder: */
  namedWindow("Edge-detected Image");
  imshow("Edge-detected Image", newImage);
  waitKey(0);
  imwrite(name + ".jpg", newImage);
}