//34567891*********2*********3*********4*********5*********6*********7*********8
// --------------------------- PaperDetector.cpp -------------------------------
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

#include "PaperDetector.h"

//four corners of detected rectangles
const int TOP_LEFT = 3;
const int TOP_RIGHT = 0;
const int BOTTOM_LEFT = 2;
const int BOTTOM_RIGHT = 1;

// PaperDetector: constructor for PaperDetector with input image
// Preconditions: newImg is a valid image file. 
// Postconditions: creates new PaperDetector
PaperDetector::PaperDetector(Mat newImg) {
	this->img = newImg;
	this->croppedImg = img;
	this->paperWidth = 8.5;
	this->paperHeight = 11;
	hasOverlayImg = false;
	objWidth = 0;
	objHeight = 0;
}

// setDetectedArea: crops img to decrease detection area towards the center
// of the image. Speeds up processing time.
// Preconditions: rectangle r is not bigger than the resized image
// Postconditions: croppedImg stores the subimage to be searched
void PaperDetector::setDetectedArea(Rect r, Mat resized) {
	this->img = resized;
	croppedImg = img(r);
}

RNG rng(12345); //for choosing random line color

// detectPaper: detects blank paper objects in the image
// Preconditions: any paper in image is white in color
// Postconditions: squares contains vectors of points for all potential paper
// pieces
void PaperDetector::detectPaper() {
	squares.clear();
	Mat pyr, timg, gray0(croppedImg.size(), CV_8U), gray;

	// down-scale and upscale the image to filter out the noise
	pyrDown(croppedImg, pyr, Size(croppedImg.cols / 2, croppedImg.rows / 2));
	pyrUp(pyr, timg, croppedImg.size());
	// find squares in every color plane of the image
	for (int c = 0; c < 3; c++) {
		int ch[] = { c, 0 };
		mixChannels(&timg, 1, &gray0, 1, ch, 1);
		// try several threshold levels
		for (int l = 0; l < N; l++) {
			if (l == 0) {
				//Canny helps to catch squares with gradient shading
				Canny(gray0, gray, 0, thresh, 5);
				// dilate canny output to remove potential
				// holes between edge segments
				dilate(gray, gray, Mat(), Point(-1, -1));
			}
			else {
				gray = gray0 >= (l + 1) * 255 / N;
			}
			// find contours and store them all as a list
			findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

			vector<Point> approx;
			vector<Vec4i> hierarchy;
			Mat drawing = Mat::zeros(gray.size(), CV_8UC3);
			for (size_t i = 0; i < contours.size(); i++) {
				// approximate contour with accuracy proportional
				// to the contour perimeter
				approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) 
					* 0.03, true);

				// square contours should have 4 vertices after approximation
				// relatively large area (to filter out noisy contours)
				// and be convex.
				// Note: absolute value of an area is used because
				// area may be positive or negative - in accordance with the
				// contour orientation
				if (approx.size() == 4
					&& fabs(contourArea(Mat(approx))) > 1000
					&& isContourConvex(Mat(approx))) {
					double maxCosine = 0;
					for (int j = 2; j < 5; j++) {
						// find the maximum cosine of the angle between joint edges
						double cosine = (double)fabs(angle(approx[j % 4], 
							approx[j - 2], approx[j - 1]));
						maxCosine = MAX(maxCosine, cosine);
					}
					// if cosines of all angles are small
					// (all angles are ~90 degree) then write quandrange
					// vertices to resultant sequence
					if (maxCosine < 0.5) {
						squares.push_back(approx);
					}
				}
			}
		}
	}

	eliminateFalsePositives();
	calculatePaperDimensions();
}

// calculatePaperDimensions: sets the pixel height and width for the 
// identified paper (if it exists)
// Preconditions: None
// Postconditions: sets object dimensions to their stored values, or zero if
// no paper object has been identified
void PaperDetector::calculatePaperDimensions() {
	if (paperSquares.size() > 0) {
		const Point tl = paperSquares[0][TOP_LEFT];
		const Point tr = paperSquares[0][TOP_RIGHT];
		const Point bl = paperSquares[0][BOTTOM_LEFT];
		const Point br = paperSquares[0][BOTTOM_RIGHT];
		objWidth = abs(tl.x - br.x);
		objHeight = abs(tl.y - br.y);
	}
	else {
		objWidth = 0;
		objHeight = 0;
		cout << "No paper detected. Height will be 0." << endl;
	}
}

// eliminateFalsePositives: eliminates potential paper objects identified
// if their average color is too dark
// Preconditions: img is a color image
// Postconditions: paperSquares contains vectors of points for each possible paper
void PaperDetector::eliminateFalsePositives() {
	//for setting object values once paper has been detected;
	for (size_t i = 0; i < squares.size(); i++) {
		const Point tl = squares[i][TOP_LEFT];
		const Point tr = squares[i][TOP_RIGHT];
		const Point bl = squares[i][BOTTOM_LEFT];
		const Point br = squares[i][BOTTOM_RIGHT];
		const Point* p = &squares[i][0];
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
			if (tr.x > bl.x && tr.y < bl.y) {
				left = bl;
				right = tr;
			}
		}

		if (p->x >= img.rows / 8 && p->y >= 2 && p->y < img.cols * 2 / 3) {
			for (int r = left.x; r < right.x; r++) {
				for (int c = left.y - 1; c >= right.y; c--) {
					for (int b = 0; b < 3; b++) {
						if (r < img.rows && c < img.cols) {
							avg += (int)img.at<Vec3b>(r, c)[b];
						}
					}
					avg /= 3; //find avg color value
					if (avg >= 180) {
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
}

// displayPaper: displays the outlined paper object
// Preconditions: None
// Postconditions: displays and outputs the image with outlined paper object.
// If not object, just displays the image.
void PaperDetector::displayPaper() {
	if (paperSquares.size() > 0) {
		const Point* p = &paperSquares[0][0];
		int n = (int)paperSquares[0].size();
		polylines(croppedImg, &p, &n, 1, true, Scalar(0, 255, 0), 2, LINE_AA);
	}
}

// overlayImage: overlays the detected paper with a image from the file
// Preconditions: None
// Postconditions: image is overlayed the identified paper's location in
// the cropped image
void PaperDetector::overlayImage(Mat overlay) {
	if (paperSquares.size() > 0) {
		const Point tl = paperSquares[0][TOP_LEFT];
		const Point tr = paperSquares[0][TOP_RIGHT];
		const Point bl = paperSquares[0][BOTTOM_LEFT];
		const Point br = paperSquares[0][BOTTOM_RIGHT];
		Mat resizedOverlay;
		Point left = tl;

		// tl at bl
		if (tl.x < br.x && tl.y > br.y) {
			left = bl;
			if (tr.x < bl.x && tr.y < bl.y) {
				left = tr;
			}
		}
		// tl at br
		else if (tl.x > br.x&& tl.y > br.y) {
			left = br;
		}
		// tl at tr
		else if (tl.x > br.x&& tl.y < br.y) {
			left = bl;
			if (tr.x < bl.x && tr.y < bl.y) {
				left = tr;
			}
		}

		resize(overlay, resizedOverlay, Size(objWidth, objHeight));
		resizedOverlay.copyTo(croppedImg(Rect(left.x, left.y, resizedOverlay.cols, 
			resizedOverlay.rows)));
	}
}

// overlayImage: overlays the detected paper with a string containing the
// calculated height
// Preconditions: None
// Postconditions: text of the height is overlayed the identified paper's location 
// in the cropped image 
void PaperDetector::overlayImage(String howTall) {
	if (paperSquares.size() > 0) {
		const Point tl = paperSquares[0][TOP_LEFT];
		const Point tr = paperSquares[0][TOP_RIGHT];
		const Point bl = paperSquares[0][BOTTOM_LEFT];
		const Point br = paperSquares[0][BOTTOM_RIGHT];
		Point left = tl, right = br;

		// tl at bl
		if (tl.x < br.x && tl.y < br.y) {
			left = bl;
			right = tr;
		}
		// tl at br
		else if (tl.x > br.x&& tl.y < br.y) {
			left = br;
			right = tl;
		}
		// tl at tr
		else if (tl.x > br.x&& tl.y > br.y) {
			left = tr;
			right = bl;
		}

		Point p = Point(left.x - (left.x - right.x) / 2 - howTall.length() * 3, 
			left.y - (left.y - right.y) / 2);
		putText(croppedImg, howTall, p, FONT_HERSHEY_DUPLEX, 0.3, Scalar(255, 191, 0), 1);
	}
}

// setPaperDimensionsInInches: sets the paper's dimensions in real life, input
// by the user
// Preconditions: all parameters are positive values
// Postconditions: values are stored in paperWidth and paperHeight
void PaperDetector::setPaperDimensionsInInches(double width, double height) {
	this->paperWidth = width;
	this->paperHeight = height;
}

// angle: returns angles between the four points
double PaperDetector::angle(Point pt1, Point pt2, Point pt0) {
	double dx1 = (double)pt1.x - pt0.x;
	double dy1 = (double)pt1.y - pt0.y;
	double dx2 = (double)pt2.x - pt0.x;
	double dy2 = (double)pt2.y - pt0.y;
	return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) 
		* (dx2 * dx2 + dy2 * dy2) + 1e-10);
}

// getPaperWidth: getter for paperWidth
double PaperDetector::getPaperWidth() {
	return paperWidth;
}

// getPaperHeight: getter for paperHeight
double PaperDetector::getPaperHeight() {
	return paperHeight;
}

// getObjectWidth: getter for objWidth
int PaperDetector::getObjectWidth() {
	return objWidth;
}

// getObjectHeight: getter for objHeight
int PaperDetector::getObjectHeight() {
	return objHeight;
}


