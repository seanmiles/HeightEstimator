#ifndef PAPERDETECTOR_H
#define PAPERDETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace std;

class PaperDetector {

private:
	Mat edged;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	vector<vector<Point> > squares;
	Mat img;

public:
	PaperDetector(Mat);
	void detectPaper();
	void displayPaper();
	double angle(Point, Point, Point);
};

#endif