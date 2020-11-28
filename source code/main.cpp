// main.cpp
// Author: 

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <math.h>
#include <map>
using namespace cv;
using namespace std;




// main:
// precondition: 
// postconditions: 
int main(int argc, char* argv[])
{	
		Mat image = imread("test3.jpg");	
		namedWindow("Original Image");
		imshow("Original Image", image);
		waitKey(0);

}