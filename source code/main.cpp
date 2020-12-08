/* Authors: Garima Maheshwari and Hailey Schauman
 * Date: 11/30/2020
 *
 * Description: Used to execute a set of tests that determine if a given search
 * image has an exemplar. */
//#include "tests.hpp"
#include "helperFunctions.hpp"
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
  Mat exemplarFV = imread("cottonMaskFV.jpg");
  readImage(exemplarFV, "Exemplar: Front View");
  trimImage(exemplarFV);

  /*TESTING IMAGE CROPPING*/

  // Reading person image:
  Mat person1FV = imread("person1.jpg");
  readImage(person1FV, "Person 1: front view");
  trimImage(person1FV);

  // Reading person 2 image:
   //Mat person2FV = imread("person2.jpg");
   //readImage(person2FV, "Person 2: front view");
   //trimImage(person2FV);


   Mat noperson = imread("noMaskPerson.jpg");
   readImage(noperson, "Person 1: no mask");
   trimImage(noperson);


  /* NEGATIVE IMAGES */
   //Mat butterfly = imread("butterfly.jpg");
   //readImage(butterfly, "NEGATIVE Image");
   //trimImage(butterfly);

  // Call smaller image function:

  ObjectRecognition exemplar(exemplarFV);


    exemplar.transformationSpace();
 // exemplar.printTransformationSpace();
 // exemplar.match(person1FV);
 //  exemplar.match(person2FV);
  //  exemplar.match(butterfly);
    exemplar.match(noperson);

   //Mat test = imread("cottonMaskFV.jpg");
   //readImage(test, "Exemplar: Front View");
   //trimImage(test);
   //exemplar.match(test);
   


  // exemplar.printTransformationSpace();
  // Mat exemplarLV = imread("cottonMaskLV.jpg");
  // readImage(exemplarLV, "Exemplar: Left View");

  // Mat exemplarRV = imread("cottonMaskRV.jpg");
  // readImage(exemplarRV, "Exemplar: Right View");

  /* Test front-view cotton masks. */
  // cottonMaskTestFV();
  ///* Test left-view cotton masks. */
  // cottonMaskTestLV();
  ///* Test right-view cotton masks. */
  // cottonMaskTestRV();

  /* N95 MASKS */

  return 0;
}