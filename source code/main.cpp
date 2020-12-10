/* Authors: Garima Maheshwari and Hailey Schauman
 * Date: 11/30/2020
 *
 * Description: Used to execute a set of tests that determine if a given search
 * image has a match with an exemplar. */
#include "tests.hpp"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

/* Purpose: Call all tests in tests.hpp.
 * Pre-conditions: None.
 * Post-conditions: Pass expected tests. */
int main(int argc, char *argv[]) {

  /* Call test function that passes images with masks: */
  cottonMaskTestFVPos();
  cout << endl;
  /* Call test function that passes images with no masks: */
  cottonMaskTestFVNeg();
  cout << endl;
  /* Testing done to show false negatives, false positives, and/or incorrect
   * output images: */
  cottonMaskFVIncorrectResults();

  return 0;
}