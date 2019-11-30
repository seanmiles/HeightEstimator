// main.cpp
// Purpose
// Author: Sean Miles, Bradley Pratt, Tung Dinh
// CSS 487 - Fall Quarter 2019

#include "PeopleDetector.h"
#include "PaperDetector.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;

// main
// precondition: A
// postconditions: A
int main(int argc, char* argv[])
{
	Mat img = imread("test3.jpg");
	Mat overlay = imread("overlay.jpg");
	if (!img.data) return -1;
	if (!overlay.data) return -1;

	PaperDetector paperDetector(img);
	paperDetector.detectPaper();
	paperDetector.setPaperDimensionsInInches(11, 8.5);
	paperDetector.overlayImage(overlay);
	paperDetector.displayPaper();

    PeopleDetector peopleDetector(img);
    peopleDetector.detectPeople();
    peopleDetector.displayPeople();

    return 0;
}