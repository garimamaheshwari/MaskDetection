/* Authors: Garima Maheshwari and Hailey Schauman
 * Date: 11/30/2020
 *
 * Description: A .hpp file that has multiple tests for the object recognition.
 * Contains tests for the following:
 *      COTTON MASK: front-view, left-view, and right-view.
 *      N95 MASK: front-view, left-view, and right-view. 
 * Uses a variety of search images that have a variety of colors and features. */
#include "helperFunctions.hpp"
#include <assert.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

/* Purpose: Test front-view cotton mask with multiple search images.
 * Pre-conditions: None.
 * Post-conditions: Passes all tests. */
void cottonMaskTestFV() {
  /* Read in front-view mask exemplar. */
  Mat exemplar = imread("cottonMaskFV.jpg");

  /* Perform edge detection on the exemplar image. */
  readImage(exemplar, "Exemplar Front-View");

  /* Read in positive search image. */
  Mat positive1FV = imread("person1.jpg");
  Mat positive2FV = imread("person2.jpg");

  /* Read in negative search image. */
  Mat falseFV = imread("");

  /* Perform edge detection on the search images. */
  readImage(positive1FV, "Person1 wearing mask (front-view)");
  readImage(positive2FV, "Person2 wearing mask (front-view)");

  /* Create objectRecognition object for exemplar. */
  ObjectRecognition cottonMask(exemplar);

  /* Test positive images. */
  assert(cottonMask.match(positive1FV));
  assert(cottonMask.match(positive2FV));

  /* Test negative image. */
  assert(!cottonMask.match(falseFV));
}

/* Purpose: Test left-view cotton mask with multiple search images.
 * Pre-conditions: None.
 * Post-conditions: Passes all tests. */
void cottonMaskTestLV() {
  /* Read in left-view mask exemplar. */
  Mat exemplar = imread("cottonMaskLV.jpg");

  /* Perform edge detection on the exemplar image. */
  readImage(exemplar, "Exemplar Left-View");

  /* Read in positive search image. */
  Mat positive = imread("");

  /* Read in negative search images. */
  Mat falseLV = imread("");
  Mat falseRV = imread("");

  /* Perform edge detection on the search images. */
  readImage(positive, "Person wearing mask (left-view)");
  readImage(falseLV, "FALSE (left-view)");
  readImage(falseRV, "FALSE (right-view)");

  /* Create objectRecognition object for exemplar. */
  ObjectRecognition cottonMask(exemplar);

  /* Test positive image. */
  assert(cottonMask.match(positive));

  /* Test negative images. */
  assert(!cottonMask.match(falseLV));
  assert(!cottonMask.match(falseRV));
}

/* Purpose: Test right-view cotton mask with multiple search images.
 * Pre-conditions: None.
 * Post-conditions: Passes all tests. */
void cottonMaskTestRV() {
  /* Read in right-view mask exemplar. */
  Mat exemplar = imread("cottonMaskRV.jpg");

  /* Perform edge detection on the exemplar image. */
  readImage(exemplar, "Exemplar Right-View");

  /* Read in positive search image. */
  Mat positive = imread("");

  /* Read in negative search images. */
  Mat falseRV = imread("");
  Mat falseLV = imread("");

  /* Perform edge detection on the search images. */
  readImage(positive, "Person wearing mask (right-view)");
  readImage(falseRV, "FALSE (right-view)");
  readImage(falseLV, "FALSE (left-view)");

  /* Create objectRecognition object for exemplar. */
  ObjectRecognition cottonMask(exemplar);

  /* Test positive image. */
  assert(cottonMask.match(positive));

  /* Test negative images. */
  assert(!cottonMask.match(falseRV));
  assert(!cottonMask.match(falseLV));
}