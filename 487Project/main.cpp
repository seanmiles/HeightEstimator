// main.cpp
// Purpose
// Author: Sean Miles, Bradley Pratt, Tung Dinh
// CSS 487 - Fall Quarter 2019

#include "PeopleDetector.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;
RNG rng(12345);

// main
// precondition: A
// postconditions: A
int main(int argc, char* argv[])
{
	Mat img = imread("test.jpg");
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

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(edged, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	// Get the moments
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++) {
		mu[i] = moments(contours[i], false);
	}
	// Get the mass centers
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++) {
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}

	// Draw contours
	Mat drawing = Mat::zeros(edged.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		circle(drawing, mc[i], 4, color, -1, 8, 0);
	}
	cout << "Center of contours = " << mc[0] << endl;

	// Find contours in edge map
	namedWindow("Picture");
	imshow("Picture", drawing);
	imwrite("contours_out.jpg", drawing);
	waitKey(0);

    PeopleDetector peopleDetector(img);
    peopleDetector.detectPeople();
    peopleDetector.displayPeople();

    return 0;
}