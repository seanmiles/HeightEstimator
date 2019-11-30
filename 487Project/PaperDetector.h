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
	double paperWidth;
	double paperHeight;
	Mat img;

public:
	PaperDetector(Mat);
	void detectPaper();
	void overlayImage(Mat);
	void displayPaper();
	void setPaperDimensionsInInches(double, double);
	// Helper methods
	double angle(Point, Point, Point);
	double inchToPixel(double);
};

#endif