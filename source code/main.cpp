/* Authors: Garima Maheshwari and Hailey Schauman
 * Date: 11/30/2020
 *
 * Description: Used to execute a set of tests that determine if a given search
 * image has an exemplar. */
#include "tests.hpp"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

/* Purpose: Call all tests in tests.hpp.
 * Pre-conditions: None.
 * Post-conditions: Pass all tests. */
int main(int argc, char *argv[]) {

  /* COTTON MASKS */

  /* Test front-view cotton masks. */
  cottonMaskTestFV();
  /* Test left-view cotton masks. */
  cottonMaskTestLV();
  /* Test right-view cotton masks. */
  cottonMaskTestRV();

  /* N95 MASKS */

  return 0;
}