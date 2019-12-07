//34567891*********2*********3*********4*********5*********6*********7*********8
// ------------------------------- main.cpp ------------------------------------
// Tung Dinh, Sean Miles, & Bradley Pratt CSS 487
// Created: 11/21/19
// Last Edit: 12/6/19
// -----------------------------------------------------------------------------
// Purpose - Main driver for our 487 final project to calculate the height of
// a person in a provided image based of their recognized pixel height and that
// of a recognized piece of paper.
// -----------------------------------------------------------------------------
// Assumptions - Input image is a valid image file. Paper is at the same depth
// as the person holding it is. Paper is blank and white. Dimensions are 
// 8.5" x 11".
// -----------------------------------------------------------------------------

#include "PeopleDetector.h"
#include "PaperDetector.h"
#include "HeightCalculator.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// main method
// Preconditions: overlay.jpg exists in the code directory and is a valid JPG
// Postconditions: Two images are displayed on the screen. The first shows
// just the people detector box, and the second will display both the people 
// and paper boxes, along with the calculated height displayed on the paper if
// the user desires.
int main(int argc, char* argv[]) {
	//Get input from user
	string filename;
	cout << "Please input filename you would like to test." << endl;
	cin >> filename;

	string system;
	cout << "Would you like height displayed in meters or feet? (F or M)" << endl;
	cin >> system;

	string overlayHeight;
	cout << "Would you like an image ('overlay.jpg') overlayed onto the paper? (Y or N)" << endl;
	cin >> overlayHeight;

	char measure = system.at(0);
	char dispOverlay = overlayHeight.at(0);

	Mat img = imread(filename);
	if (!img.data) return -1;
	
	// Detections
	PeopleDetector people(img);
	people.detectPeople();
	PaperDetector paper(img);
	paper.setDetectedArea(people.getDetectedRect(), people.getDetectedImg());
	paper.detectPaper();

	//Height Calculations
	HeightCalculator height(paper.getPaperHeight(), paper.getPaperWidth(),
		paper.getObjectHeight(), paper.getObjectWidth(), people.getHeight());
	String howTall;
	if (measure == 'F' || measure == 'f') {
		howTall = height.getHeightInFeet();
	}
	else if (measure == 'M' || measure == 'm') {
		howTall = height.getHeightInMeters();
	}
	cout << height.getHeightInFeet() << endl;
	cout << height.getHeightInMeters() << endl;

	// Overlay Image
	if (dispOverlay == 'Y' || dispOverlay == 'y') {
		Mat overlay = imread("overlay.jpg");
		if (!overlay.data) return -1;
		paper.overlayImage(overlay);
	}
	else if (dispOverlay == 'N' || dispOverlay == 'n') {
		paper.overlayImage(howTall);
	}

	// Display
	paper.displayPaper();
	people.displayPeople();

	waitKey(0);
	return 0;
}