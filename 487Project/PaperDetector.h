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
	Mat img;

public:
	PaperDetector(Mat img);
	void detectPaper();
	void displayPaper();
};

#endif