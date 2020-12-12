/* Authors: Garima Maheshwari and Hailey Schauman
 * Date: 11/30/2020
 *
 * Description: A .hpp file that has multiple tests for the object recognition.
 * Contains tests for cotton-masks with negative and positive tests.
 * Uses a variety of search images that have a variety of colors and features.
 */
#include "helperFunctions.hpp"
#include <assert.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

/* Purpose: Positive testing on front-view cotton mask with search images.
 * Pre-conditions: None.
 * Post-conditions: Passes the test successfully. */
void cottonMaskTestFVPos() {
  /* EXEMPLAR */

  /* Read in front-view mask exemplar: */
  Mat exemplar = imread("cottonMaskFV.jpg");

  /* Make clones of mask exemplar: */
  Mat originalEx = exemplar.clone();
  Mat edgedEx = exemplar.clone();

  /* Perform edge detection on the exemplar image: */
  readImage(edgedEx, "Exemplar Image (front-view)");
  /* Crop images: */
  trimImage(edgedEx, originalEx);

  /* POSITIVE TESTING */

  /* Create objectRecognition object for exemplar: */
  ObjectRecognition cottonMask(edgedEx);

  /* Create transformation space: */
  cottonMask.transformationSpace();

  /* Test positive image: */
  assert(cottonMask.match(edgedEx, originalEx, "truePositive"));
}

/* Purpose: Negative testing on front-view cotton mask with a search image.
 * Pre-conditions: None.
 * Post-conditions: Passes the tests successfully. */
void cottonMaskTestFVNeg() {
  /* EXEMPLAR */

  /* Read in front-view mask exemplar: */
  Mat exemplar = imread("cottonMaskFV.jpg");

  /* Make clone of mask exemplar: */
  Mat edgedEx = exemplar.clone();

  /* Perform edge detection on the exemplar image: */
  readImage(edgedEx, "Exemplar Image (front-view)");
  /* Crop images: */
  trimImage(edgedEx, exemplar);

  /* Read in negative search image: */
  Mat originalFalse = imread("personWithNoMask.jpg");
  Mat falseFV = originalFalse.clone();

  /* Read in image and crop images: */
  readImage(falseFV, "Person not wearing mask (front-view)");
  trimImage(falseFV, originalFalse);

  /* Create objectRecognition object for exemplar: */
  ObjectRecognition cottonMask(exemplar);

  /* Create transformation space: */
  cottonMask.transformationSpace();

  /* Test negative image: */
  assert(!cottonMask.match(falseFV, originalFalse, "trueFalse"));
  cout << endl;

  /* Read in negative search image: */
  originalFalse = imread("personWithNoMask2.jpg");
  falseFV = originalFalse.clone();

  /* Read in image and crop images: */
  readImage(falseFV, "Person2 not wearing mask (front-view)");
  trimImage(falseFV, originalFalse);

  /* Test negative image: */
  assert(!cottonMask.match(falseFV, originalFalse, "trueFalse2"));
}

/* Purpose: To illustrate false negatives and positives.
 * Pre-conditions: None.
 * Post-conditions: Passes the assert statements on images. */
void cottonMaskFVIncorrectResults() {
  /* EXEMPLAR */

  /* Read in front-view mask exemplar: */
  Mat exemplar = imread("cottonMaskFV.jpg");

  /* Make clone of mask exemplar: */
  Mat edgedEx = exemplar.clone();

  /* Perform edge detection on the exemplar image: */
  readImage(edgedEx, "Exemplar Image (front-view)");
  /* Crop images: */
  trimImage(edgedEx, exemplar);

  /* FALSE POSITIVES */

  /* Read in a mask search image: */
  Mat originalPos = imread("person1.jpg");
  Mat falsePos = originalPos.clone();

  /* Read in image and crop images: */
  readImage(falsePos, "Person wearing mask (front-view)");
  trimImage(falsePos, originalPos);

  /* Create objectRecognition object for exemplar: */
  ObjectRecognition cottonMask(edgedEx);

  /* Create transformation space: */
  cottonMask.transformationSpace();

  /* Test false negative image. Expected to get close to the mask, but return
   * false even if true: */
  assert(cottonMask.match(falsePos, originalPos, "falsePos(incorrectMatch)"));
  cout << endl;

  /* Read in images: */
  originalPos = imread("butterfly.jpg");
  falsePos = originalPos.clone();

  /* Read in image and crop images: */
  readImage(falsePos, "Butterfly");
  trimImage(falsePos, originalPos);

  /* Test false negative image. Expected to get close to the mask, but return
   * false even if true: */
  assert(cottonMask.match(falsePos, originalPos, "falsePos(noMask)"));
  cout << endl;

  /* FALSE NEGATIVES */

  /* Read in a mask search image: */
  Mat originalNeg = imread("person2.jpg");
  Mat falseNeg = originalNeg.clone();

  /* Read in image and crop images: */
  readImage(falseNeg, "Person wearing mask (front-view)");
  trimImage(falseNeg, originalNeg);

  /* Test false negative image. Expected to get close to the mask, but return
   * false even if true: */
  assert(!cottonMask.match(falseNeg, originalNeg, "falseNeg"));
  cout << endl;
}