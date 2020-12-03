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

/* Purpose: Destructor to remove dynamic memory.
 * Pre-conditions: None.
 * Post-conditions: Deletes Transformations structs in transformCombinations. */
ObjectRecognition::~ObjectRecognition() {
  /* Iterate through the 3D vector to remove dynamically allocated struct
   * objects. */
  for (int row = 0; row < transformCombinations.size(); ++row) {
    for (int col = 0; col < transformCombinations[row].size(); ++col) {
      for (int depth = 0; depth < transformCombinations[row][col].size();
           ++depth)
        delete transformCombinations[row][col][depth];
    }
  }
}

/* Purpose: To perform object recognition on an exemplar and searchImage.
 * Pre-conditions: searchImage is a valid image (e.g., not .gif).
 * Post-conditions: Returns true if the exemplar is found in the image. */
bool ObjectRecognition::match(Mat &searchImage) { return false; }

/* Purpose: To create a transformation space for the exemplar.
 * Pre-conditions: None.
 * Post-conditions: Creates a transformation space for scaling/rotation. */
void ObjectRecognition::transformationSpace() {
  /* Initialize dimension size for transformation space. */
  int xScale = dimensionSize(maxXScale, incrementScale);
  int yScale = dimensionSize(maxYScale, incrementScale);
  int rotationScale = dimensionSize(maxRotation, incrementRotation);

  /* Calculate the transformation combinations per (row, col, z). */
  double xIncrement = 0;
  double yIncrement = 0;
  int rotation = 0;

  /* Iterate through, calculating the xScale, yScale, and rotation combinations.
   * This is stored inside a 3D vector. */
  for (int row = 0; row < xScale; ++row) {
    vector<vector<Transformations *>> transformSpace;
    for (int col = 0; col < yScale; ++col) {
      vector<Transformations *> temp;
      for (int depth = 0; depth < rotationScale; ++depth) {

        /* Create transformation combination for the transformation space. */
        Transformations *newTransformCombo = new Transformations;
        newTransformCombo->xScale = xIncrement;
        newTransformCombo->yScale = yIncrement;
        newTransformCombo->rotation = rotation;

        /* Increment rotation for next iteration. */
        rotation += incrementRotation;
        temp.push_back(newTransformCombo);
      }

      /* Increment yScale for next iteration. */
      yIncrement += incrementScale;
      rotation = 0;
      transformSpace.push_back(temp);
    }

    /* Increment xScale for next iteration. */
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
      for (int depth = 0; depth < transformCombinations[row][col].size(); ++depth) {
        cout << "ROW: " << row << " COL: " << col << " DEPTH: " << depth;
        cout << "\tRotation: " << transformCombinations[row][col][depth]->rotation;
        cout << "\txScale: " << transformCombinations[row][col][depth]->xScale;
        cout << "\tyScale: " << transformCombinations[row][col][depth]->yScale
             << endl;
      }
    }
  }
}

/* Purpose: To calculate the dimension size of a given transformation axis.
 * Pre-conditions: none.
 * Post-conditions: Returns a number that corresponds to an axis' size for the
 *          transformation space. */
int ObjectRecognition::dimensionSize(double transform, double increment) const {
  double size = transform / increment;

  /* If the size has a decimal greater than .5, increment the size by 1 to keep
   * important data when doing floor() conversion. Otherwise, return the size
   * that has been down casted to a whole number. */
  if (size - floor(size) > 0.5)
    return static_cast<int>(size) + 1;

  return static_cast<int>(size);
}
