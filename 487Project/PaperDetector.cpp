#include "PaperDetector.h"

RNG rng(12345);

PaperDetector::PaperDetector(Mat img) {
	this->img = img;
}

void PaperDetector::detectPaper() {

	Mat gray;
	// Convert to grayscale
	cvtColor(img, gray, COLOR_BGR2GRAY);
	// Smooth it
	GaussianBlur(gray, gray, Size(7, 7), 0);

	// Edge detection
	Canny(gray, edged, 50, 100);
	// Dilation
	dilate(edged, edged, NULL);
	// Erosion
	erode(edged, edged, NULL);

	findContours(edged, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

}

void PaperDetector::displayPaper() {

	if (countNonZero(edged) < 1) {
		namedWindow("Picture");
		imshow("Picture", img);
		imwrite("contours_out.jpg", img);
	}
	else {
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

		namedWindow("Picture");
		imshow("Picture", drawing);
		imwrite("contours_out.jpg", drawing);
	}
}