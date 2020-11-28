// main.cpp
// Author: 

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#include <map>
#include "Functions.hpp"

using namespace cv;
using namespace std;




// main:
// precondition: 
// postconditions: 
int main(int argc, char* argv[])
{	
	 // Read in exemplar image (frontview of mask)
		 Mat exemplar = imread("frontView.jpg");
         readImage(exemplar, "Exemplar Front View");

	// Read in person wearing mask (frontview)
         Mat person1 = imread("person1.jpg");
         readImage(person1, "Person1 wearing mask(front view)");

    // Read in another person wearing mask (frontview)
         Mat person2 = imread("person2.jpg");
         readImage(person2, "Person2 wearing mask(front view)");




}