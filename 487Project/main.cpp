// main.cpp
// Purpose
// Author: Sean Miles, Bradley Pratt, Tung Dinh
// CSS 487 - Fall Quarter 2019

#include "PeopleDetector.h"
#include "PaperDetector.h"
#include "HeightCalculator.h"
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
	Mat img = imread("Blake.jpg");
	Mat overlay = imread("overlay.jpg");
	if (!img.data) return -1;
	if (!overlay.data) return -1;

	// Detections
	PeopleDetector people(img);
	people.detectPeople();
	PaperDetector paper(img);
	paper.setDetectedArea(people.getDetectedRect(), people.getDetectedImg());
	paper.detectPaper();

	//Height Calculations
	HeightCalculator height(paper.getPaperHeight(), paper.getPaperWidth(),
		paper.getObjectHeight(), paper.getObjectWidth(), people.getHeight());
	String howTall = height.getHeightInFeet();

	cout << height.getHeightInFeet() << endl;
	cout << height.getHeightInMeters() << endl;

	// Overlay Image
	//paper.overlayImage(overlay);
	paper.overlayImage(howTall);
	// Display
	paper.displayPaper();
	people.displayPeople();

	waitKey(0);
	return 0;
}