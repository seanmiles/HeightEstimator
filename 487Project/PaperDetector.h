//34567891*********2*********3*********4*********5*********6*********7*********8
// ---------------------------- PaperDetector.h --------------------------------
// Tung Dinh, Sean Miles, & Bradley Pratt CSS 487
// Created: 11/21/19
// Last Edit: 12/6/19
// -----------------------------------------------------------------------------
// Purpose - Utilizes opencv methods to identify a blank paper pieces in an 
// image input by user. Displays a rectangle around the paper identified.
// -----------------------------------------------------------------------------
// Assumptions - Input image is a valid image file. Paper is at the same depth
// as the person holding it is. Paper is blank and white. Dimensions are 
// 8.5" x 11".
// -----------------------------------------------------------------------------
#ifndef PAPERDETECTOR_H
#define PAPERDETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class PaperDetector {
public:
	//Constructor
	PaperDetector(Mat);

	//Helper Functions
	void detectPaper();
	void overlayImage(Mat);
	void overlayImage(String);
	void displayPaper();
	double angle(Point, Point, Point);

	//Getters & Setters
	void setPaperDimensionsInInches(double, double);
	void setDetectedArea(Rect, Mat);
	double getPaperWidth();
	double getPaperHeight();
	int getObjectWidth();
	int getObjectHeight();

private:
	const int DETECT_HEIGHT = 400; //sets constant height to match other classes
	vector<vector<Point>> contours;
	vector<vector<Point>> squares;
	vector<vector<Point>> paperSquares;
	double paperWidth; //user input width (inches)
	double paperHeight; //user input height (inches)
	int objWidth; //recognized object width in pixels
	int objHeight; //recognized object height in pixels
	Mat img;
	Mat croppedImg;
	bool hasOverlayImg;
	int thresh = 50, N = 16;

	void calculatePaperDimensions();
	void eliminateFalsePositives();
};

#endif