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
  
  /*Count number of edges in exemplar image*/
  exemplarEdges = 0;
  for (int row = 0; row < exemplar.rows; ++row) {
    for (int col = 0; col < exemplar.cols; ++col) {
      if (exemplar.at<uchar>(row, col) == edge) {
        exemplarEdges++;
      }
    }
  }

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
 * Pre-conditions: searchImage is a valid image (e.g., not .gif) that has already been cropped
 * Post-conditions: Returns true if the exemplar is found in the image. */
bool ObjectRecognition::match(Mat &searchImage) { 
    //Divide and Conquer variables:
    int bucketIncrement = 20;
    int greatestCount = 0;
    

    //iterate through the number of translations
    for (int row = 50; row < 55; row += 5) {
      for (int col = 50; col < 55; col += 5) {

            // iterate through the transformation space
        int greatestCombo = 0;
            int r = transformCombinations.size();
        int c = transformCombinations[0].size();
            int d = transformCombinations[0][0].size();
        int total = r * d * c;
            for (int rowT = 0; rowT < transformCombinations.size(); ++rowT) {
              for (int colT = 0; colT < transformCombinations[rowT].size();
                   ++colT) {
                for (int depth = 0;
                     depth < transformCombinations[rowT][colT].size(); ++depth) {
                  pair<double, double> scale =
                      make_pair(transformCombinations[rowT][colT][depth]->xScale,
                                transformCombinations[rowT][colT][depth]->yScale);
                  pair<double, double> origin = make_pair(row,col);
                  int count =
                      getCount(searchImage, scale,
                               transformCombinations[rowT][colT][depth]->rotation,origin);
                  if (count > greatestCombo) {
                    greatestCombo = count;
                  }
                }
              }
            }
            /*check if greatestCombo is a good enough value*/
            if (greatestCombo > greatestCount) {
              greatestCount = greatestCombo;
            }
            //if not: change the translation bucket

      }
    }
    /*Within exemplar edge range, return true*/
    cout << "Exemplar Edges: " << exemplarEdges << endl;
    cout << "Count: " << greatestCount << endl;

    int edgeThreshold = 20;

    return (greatestCount > exemplarEdges - edgeThreshold &&
            greatestCount < exemplarEdges + edgeThreshold);
     }
int ObjectRecognition::getCount(Mat &searchImage, pair<double, double> scale, int rotation,
                                pair<int, int> origin) const {
  int count = 0;
  // iterate through the searchImage:
  //for (int row = origin.first; row < searchImage.rows; ++row) {
  //  for (int col = origin.second; col < searchImage.cols; ++col) {
        //iterate through the exemplar:
      for (int rowEx = 0; rowEx < exemplar.rows; ++rowEx) {
        for (int colEx = 0; colEx < exemplar.cols; ++colEx) {
            //transform exemplar point to match to search image:
          if (exemplar.at<uchar>(rowEx, colEx) != edge) {
              continue;
          }
          //set the exemplar point w/ respect to origin
            double newRow = rowEx + origin.first;
            double newCol = colEx + origin.second;
            
          //perform scale
            newRow = newRow / scale.second;
            newCol = newCol / scale.first;

           //perform rotation:
            // convert degrees to radians and find cos and sin values with the
            // given radians 
            const double pi = 3.14159265;
            double cosVal = cos(rotation * (pi / 180));
            double sinVal = sin(rotation * (pi / 180));

            double radianCol = (cosVal * newCol) + (sinVal * newRow);
            double radianRow = (-sinVal * newCol) + (cosVal * newRow);

            newCol = radianCol;
            newRow = radianRow;
            //check if in bounds
            if (newCol >= 0 && newCol < searchImage.cols && newRow >= 0 &&
                newRow < searchImage.rows) {
                //check if edge:
              if (searchImage.at<uchar>(newRow, newCol) == edge) {
                  count++;
              }
            }
      //  }
      //}
    }
  }
  return count;
    }

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


