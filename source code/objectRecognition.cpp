#include "objectRecognition.h"




objectRecongition::objectRecongition(const Mat& exemplar)
    : exemplar(exemplar.clone()), maxXScale(maxPixelValue / exemplar.rows),
      maxYScale(maxPixelValue / exemplar.cols){
  //translations = exemplar.clone();
  //creating 3D array 
  
  
  int dims[] = {maxXScale / incrementScale, maxYScale / incrementScale, 360 / incrementRotation };
  // create 3D histogram of integers initialized to zero
  Mat hist(3, dims, CV_32S, Scalar::all(0));

  for (int row = 0; row < foreground.rows; row++) {
    for (int col = 0; col < foreground.cols; col++) {
      // foreground.at<Vec3b>(r, c)[b]
      int r = foreground.at<Vec3b>(row, col)[2] / bucketSize;
      int g = foreground.at<Vec3b>(row, col)[1] / bucketSize;
      int b = foreground.at<Vec3b>(row, col)[0] / bucketSize;

      hist.at<int>(r, g, b) += 1;
    }
  }
  
 


    
};

bool objectRecongition::match(Mat &searchImage) { return false; }

void objectRecongition::transformationSpace() { 


  for (int row = 0; row < foreground.rows; row++) {
    for (int col = 0; col < foreground.cols; col++) {

    }
  }

}
