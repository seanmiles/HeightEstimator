#ifndef PAPERDETECTOR_H
#define PAPERDETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;

class PaperDetector {

private:
	vector<vector<Point>> contours;
	vector<vector<Point> > squares;
	double paperWidth; //user input width (inches)
	double paperHeight; //user input height (inches)
	int objWidth; //recognized object width in pixels
	int objHeight; //recognized object height in pixels
	Mat img;
	double height;
	bool hasOverlayImg;

public:
	PaperDetector(Mat);
	void detectPaper();
	void overlayImage(Mat);
	void overlayImage(String);
	void displayPaper();
	void setPaperDimensionsInInches(double, double);
	int getHeightInFts();
	int getHeightInInches();
	// Helper methods
	double angle(Point, Point, Point);
	double inchToPixel(double);

	double getPaperWidth(); 
	double getPaperHeight(); 
	int getObjectWidth(); 
	int getObjectHeight(); 
};

#endif