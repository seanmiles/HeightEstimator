//34567891*********2*********3*********4*********5*********6*********7*********8
// --------------------------- PeopleDetector.h --------------------------------
// Tung Dinh, Sean Miles, & Bradley Pratt CSS 487
// Created: 11/21/19
// Last Edit: 12/6/19
// -----------------------------------------------------------------------------
// Purpose - Utilizes opencv methods to identify a single person in an image 
// input by user. Displays a rectangle around the person identified.
// -----------------------------------------------------------------------------
// Assumptions - Input image is a valid image file. 
// -----------------------------------------------------------------------------
#ifndef PEOPLEDETECTOR_H
#define PEOPLEDETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

class PeopleDetector {
public:
	//Constructor
	PeopleDetector(Mat);

	//Accessory Functions
	void detectPeople();
	void displayPeople();

	//Getters
	double getHeight(); 
	Rect getDetectedRect();
	Mat getDetectedImg();

private:
	const int DETECT_HEIGHT = 400; //set constant height for image (faster)
	HOGDescriptor hog; //people detector from opencv
	vector<Rect> detections; //track people detections
	vector<double> weights; 
	vector<int> indices;
	Mat img; //input image
    int NMSIndex; //track primary detection box after Non Maxima Suppression
	double personHeight; // store height of box

	void calculateHeight(); 
};

#endif