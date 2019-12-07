//34567891*********2*********3*********4*********5*********6*********7*********8
// -------------------------- HeightCalculator.h -------------------------------
// Tung Dinh, Sean Miles, & Bradley Pratt CSS 487
// Created: 11/21/19
// Last Edit: 12/6/19
// -----------------------------------------------------------------------------
// Purpose - Calculates height of person detected in image given the detection
// box dimensions and the detected paper dimensions.
// -----------------------------------------------------------------------------
// Assumptions - user input paper dimensions are correct. Detection boxes 
// accurately outline their objects.
// -----------------------------------------------------------------------------
#ifndef HEIGHTCALCULATOR_H
#define HEIGHTCALCULATOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

class HeightCalculator {
public:
	//Constructors
	HeightCalculator(int, int, double);
	HeightCalculator(double, double, int, int, double);

	//Getters & Setters
	String getHeightInFeet();
	String getHeightInMeters();
	double getFeet();
	int getInches();
	double getMeters();
	void setPaperDimensions(double, double);

private:
	double feet;	//calculated height in feet
	int inches;	//calculated height to the nearest inch
	double meters; //calculated height in meters

	//user input paper dimensions
	double inputPaperLong; //longer dimension
	double inputPaperShort; //shorter

	//dimensions of detected paper in img
	int detectedPaperLong; //longer dimension
	int detectedPaperShort; //shorter

	double boxHeight; //height of box outlining detected person

	void calculateHeight();
};
#endif