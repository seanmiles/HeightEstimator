#include "PaperDetector.h"

const int TOP_LEFT = 3;
const int TOP_RIGHT = 0;
const int BOTTOM_LEFT = 2;
const int BOTTOM_RIGHT = 1;

PaperDetector::PaperDetector(Mat img) {
	this->img = img;
	this->paperWidth = inchToPixel(8.5);
	this->paperHeight = inchToPixel(11);
	hasOverlayImg = false;
	height = 0;
	objWidth = 0;
	objHeight = 0;
}

void PaperDetector::detectPaper() {
	Mat image = img.clone();
	Mat blurred(image);
	medianBlur(image, blurred, 9);

	Mat gray0(blurred.size(), CV_8U), gray;
	
	for (int c = 0; c < 3; c++) {
		int ch[] = { c, 0 };
		mixChannels(&blurred, 1, &gray0, 1, ch, 1);

		const int threshold_level = 2;
		for (int l = 0; l < threshold_level; l++) {
			if (l == 0) {
				Canny(gray0, gray, 10, 20, 3);
				dilate(gray, gray, Mat(), Point(-1, -1));
			}
			else {
				gray = gray0 >= (l + 1) * 255 / threshold_level;
			}
			findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

			vector<Point> approx;
			for (size_t i = 0; i < contours.size(); i++) {
				approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);
				
				if (approx.size() == 4 
					&& fabs(contourArea(Mat(approx))) > 1000 
					&& isContourConvex(Mat(approx))) {
					double maxCosine = 0;
					for (int j = 2; j < 5; j++) {
						double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
						maxCosine = MAX(maxCosine, cosine);
					}
					if (maxCosine < 0.3) {
						squares.push_back(approx);
					}
				}
			}
		}
	}

}

void PaperDetector::displayPaper() {

	for (size_t i = 0; i < squares.size(); i++)
	{
		const Point* p = &squares[i][0];
		int n = (int)squares[i].size();
		polylines(img, &p, &n, 1, true, Scalar(0, 255, 0), 3, LINE_AA);
		circle(img, squares[i][3], 4, Scalar(0, 0, 255));
		const Point tl = squares[i][TOP_LEFT];
		const Point tr = squares[i][TOP_RIGHT];
		const Point bl = squares[i][BOTTOM_LEFT];
		const Point br = squares[i][BOTTOM_RIGHT];
		int w = abs(tl.x - tr.x);
		int h = abs(tl.y - bl.y);
		if (!hasOverlayImg) {
			String text =  to_string(getHeightInFts()) + "'" + to_string(getHeightInInches()) + "''";
			putText(img, text, Point(tl.x + w / 2 - text.length() * 3, tl.y + h / 2), FONT_HERSHEY_DUPLEX, 1, Scalar(255, 191, 0), 2);
		}
	}

	imshow("Picture", img);
	imwrite("paper_out.jpg", img);
}

void PaperDetector::overlayImage(Mat overlay) {
	hasOverlayImg = true;
	for (size_t i = 0; i < squares.size(); i++) {
		const Point tl = squares[i][TOP_LEFT];
		const Point tr = squares[i][TOP_RIGHT];
		const Point bl = squares[i][BOTTOM_LEFT];
		const Point br = squares[i][BOTTOM_RIGHT];
		Mat resizedOverlay;
		int w = abs(tl.x - tr.x);
		objWidth = w;
		int h = abs(tl.y - bl.y);
		objHeight = h;
		resize(overlay, resizedOverlay, Size(w, h));
		resizedOverlay.copyTo(img(Rect(tl.x, tl.y, resizedOverlay.cols, resizedOverlay.rows)));
	}
}

void PaperDetector::setPaperDimensionsInInches(double width, double height) {
	this->paperWidth = inchToPixel(width) / 4;
	this->paperHeight = inchToPixel(height) / 4;
}

int PaperDetector::getHeightInFts() {
	return (int)height;
}

int PaperDetector::getHeightInInches() {
	return 0;
}

// Helper method
double PaperDetector::angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1 * dy2) / sqrt((dx1*dx1 + dy1 * dy1)*(dx2*dx2 + dy2 * dy2) + 1e-10);
}

double PaperDetector::inchToPixel(double in) {
	return in * 96;
}

double PaperDetector::getPaperWidth() {
	return paperWidth;
}

double PaperDetector::getPaperHeight() {
	return paperHeight;
}

int PaperDetector::getObjectWidth() {
	return objWidth;
}

int PaperDetector::getObjectHeight() {
	return objHeight;
}

