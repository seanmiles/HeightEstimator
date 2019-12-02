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
	HeightCalculator(int, int, int);
	HeightCalculator(double, double, int, int, int);

	String getHeightInFeet();
	String getHeightInMeters();

	//Getters & Setters
	double getFeet();
	int getInches();
	double getMeters();
	void setPaperDimensions(double, double);

private:
	double feet;	//calculated height in feet
	int inches;	//calculated height in feet%inches
	double meters; //calculated height in meters

	//user input paper dimensions
	double inputPaperLong; //longer dimension
	double inputPaperShort; //shorter

	//dimensions of detected paper in img
	int detectedPaperLong; //longer dimension
	int detectedPaperShort; //shorter

	int boxHeight; //height of box outlining detected person

	void calculateHeight();
};
#endif