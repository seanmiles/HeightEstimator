#include "PaperDetector.h"

const int TOP_LEFT = 3;
const int TOP_RIGHT = 0;
const int BOTTOM_LEFT = 2;
const int BOTTOM_RIGHT = 1;

PaperDetector::PaperDetector(Mat img) {
	this->img = img;
	this->croppedImg = img;
	this->paperWidth = 8.5;
	this->paperHeight = 11;
	hasOverlayImg = false;
	height = 0;
	objWidth = 0;
	objHeight = 0;
	imgHeight = img.rows;
}

void PaperDetector::setDetectedArea(Rect r, Mat resized) {
	double ratioRows = (double) img.rows / (double) resized.rows;
	double ratioCols = (double) img.cols / (double) resized.cols;
	int w = r.width * ratioRows;
	int h = r.height * ratioCols;
	Rect crop(r.tl().x * ratioRows, r.tl().y * ratioCols, w, h);
	croppedImg = img(crop);
}
RNG rng(12345);
void PaperDetector::detectPaper() {
	squares.clear();
	Mat pyr, timg, gray0(croppedImg.size(), CV_8U), gray;
	// down-scale and upscale the image to filter out the noise
	pyrDown(croppedImg, pyr, Size(croppedImg.cols / 2, croppedImg.rows / 2));
	//imwrite("Down.jpg", pyr);
	pyrUp(pyr, timg, croppedImg.size());
	//imwrite("Up.jpg", timg);
	//Mat timg(croppedImg);
	//medianBlur(croppedImg, timg, 9);
	//Mat gray0(timg.size(), CV_8U), gray;
	//imwrite("blurred.jpg", blurred);
	
	for (int c = 0; c < 3; c++) {
		int ch[] = { c, 0 };
		mixChannels(&timg, 1, &gray0, 1, ch, 1);
		//string path = "gray0-mixCh" + to_string(c) + ".jpg";
		//imwrite(path, gray0);
		for (int l = 0; l < N; l++) {
			if (l == 0) {
				Canny(gray0, gray, 0, thresh, 5);
				dilate(gray, gray, Mat(), Point(-1, -1));
				//imwrite("gray-threshold0.jpg", gray);
			}
			else {
				gray = gray0 >= (l + 1) * 255 / N;
				//imwrite("gray-threshold1.jpg", gray);
			}
			findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

			vector<Point> approx;
			vector<Vec4i> hierarchy;
			Mat drawing = Mat::zeros(gray.size(), CV_8UC3);
			for (size_t i = 0; i < contours.size(); i++) {
				//Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				//drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
				approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.03, true);
				
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
			//imwrite("Contours.jpg", drawing);
		}
	}

	//for setting object values once paper has been detected;
	for (size_t i = 0; i < squares.size(); i++) {
		const Point tl = squares[i][TOP_LEFT];
		const Point tr = squares[i][TOP_RIGHT];
		const Point bl = squares[i][BOTTOM_LEFT];
		const Point br = squares[i][BOTTOM_RIGHT];
		const Point* p = &squares[i][0];
		objWidth = abs(tl.x - br.x);
		objHeight = abs(tl.y - br.y);
		int avg = 0;
		Point left = tl, right = br;
		// tl at bl
		if (tl.x < br.x && tl.y < br.y) {
			left = bl;
			right = tr;
		}
		// tl at br
		else if (tl.x > br.x && tl.y < br.y) {
			left = br;
			right = tl;
		}
		// tl at tr
		else if (tl.x > br.x && tl.y > br.y) {
			left = tr;
			right = bl;
		}
		cout << tl << ", " << br << "  " << left << ", " << right << endl;
		if (p->x > 3 && p->y > 3) {
			for (int r = left.x; r < right.x; r++) {
				for (int c = left.y - 1; c >= right.y; c--) {
					for (int b = 0; b < 3; b++) {
						if (r < img.rows && c < img.cols) {
							avg += (int)img.at<Vec3b>(r, c)[b];
						}
					}
					avg /= 3;
					if (avg >= 220) {
						avg = 0;
						paperSquares.push_back(squares[i]);
					}
					else {
						avg = 0;
					}
				}
			}
		}
		
	}
	cout << paperSquares.size() << endl;
}

void PaperDetector::displayPaper() {
	for (size_t i = 0; i < paperSquares.size(); i++)
	{
		const Point* p = &paperSquares[i][0];
		int n = (int)paperSquares[i].size();
		polylines(croppedImg, &p, &n, 1, true, Scalar(0, 255, 0), 3, LINE_AA);
	}

	imshow("Picture", croppedImg);
	imwrite("paper_out.jpg", croppedImg);
}

void PaperDetector::overlayImage(Mat overlay) {
	for (size_t i = 0; i < paperSquares.size(); i++) {
		const Point tl = paperSquares[i][TOP_LEFT];
		const Point tr = paperSquares[i][TOP_RIGHT];
		const Point bl = paperSquares[i][BOTTOM_LEFT];
		const Point br = paperSquares[i][BOTTOM_RIGHT];
		Mat resizedOverlay;
		int w = 0, h = 0;
		int x = 0, y = 0;
		if (abs(tl.x - tr.x) < 10) {
			w = abs(tl.y - tr.y);
			h = abs(tl.x - bl.x);
			x = tr.x;
			y = tr.y;
			resize(overlay, resizedOverlay, Size(h, w));
		}
		else {
			w = abs(tl.x - tr.x);
			h = abs(tl.y - bl.y);
			x = tl.x;
			y = tl.y;
			resize(overlay, resizedOverlay, Size(w, h));
		}	
		resizedOverlay.copyTo(croppedImg(Rect(x, y, resizedOverlay.cols, resizedOverlay.rows)));
	}
}

void PaperDetector::overlayImage(String howTall) {
	for (size_t i = 0; i < paperSquares.size(); i++) {
		const Point tl = paperSquares[i][TOP_LEFT];
		const Point br = paperSquares[i][BOTTOM_RIGHT];
		int w = abs(tl.x - br.x);
		int h = abs(tl.y - br.y);
		Point p = Point(tl.x + w / 2 - howTall.length() * 3, tl.y - (tl.y - br.y) / 2);
		//putText(croppedImg, howTall, p, FONT_HERSHEY_DUPLEX, 1, Scalar(255, 191, 0), 2);
	}
}

void PaperDetector::setPaperDimensionsInInches(double width, double height) {
	this->paperWidth = width;
	this->paperHeight = height;
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

int PaperDetector::getImageHeight() {
	return imgHeight;
}
