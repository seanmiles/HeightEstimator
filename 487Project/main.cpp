// main.cpp
// Purpose
// Author: Sean Miles, Bradley Pratt, Tung Dinh
// CSS 487 - Fall Quarter 2019

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
	Mat img = imread("paper.jpg");
	if (!img.data) return -1;

	Mat gray;
	// Convert to grayscale
	cvtColor(img, gray, COLOR_BGR2GRAY);
	// Smooth it
	GaussianBlur(gray, gray, Size(7, 7), 0);
	Mat edged;
	// Edge detection
	Canny(gray, edged, 50, 100);
	// Dilation
	dilate(edged, edged, NULL);
	// Erosion
	erode(edged, edged, NULL);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(edged, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	Mat drawing = Mat::zeros(edged.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	// Find contours in edge map
	namedWindow("Picture");
	imshow("Picture", drawing);
	imwrite("paper_out.jpg", drawing);
	waitKey(0);
    return 0;
}