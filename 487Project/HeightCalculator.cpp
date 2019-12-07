//34567891*********2*********3*********4*********5*********6*********7*********8
// ------------------------- HeightCalculator.cpp ------------------------------
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

#include "HeightCalculator.h"

// HeightCalculator: constructor for HeightCalculator when user input paper 
// dimensions haven't been input yet
// Preconditions: All parameter values are positive
// Postconditions: creates new HeightCalculator
HeightCalculator::HeightCalculator(int dpl, int dps, double bh) {
	feet = 0.0;
	inches = 0;
	meters = 0.0;
	inputPaperLong = 0.0;
	inputPaperShort = 0.0;
	boxHeight = bh;

	//find longest edge
	if (dpl > dps) {
		detectedPaperLong = dpl;
		detectedPaperShort = dps;
	}
	else {
		detectedPaperLong = dps;
		detectedPaperShort = dpl;
	}
}

// HeightCalculator: constructor for HeightCalculator
// Preconditions: All parameter values are positive
// Postconditions: creates new HeightCalculator
HeightCalculator::HeightCalculator(double ipl, double ips, int dpl, int dps, double bh) {
	feet = 0.0;
	inches = 0;
	meters = 0.0;
	boxHeight = bh;

	//find longest edge
	if (dpl > dps) {
		detectedPaperLong = dpl;
		detectedPaperShort = dps;
	}
	else {
		detectedPaperLong = dps;
		detectedPaperShort = dpl;
	}

	setPaperDimensions(ipl, ips); //set real life dimensions of paper found in image
}

// getHeightInFeet: gets the calculated in feet and remaining inches
// Preconditions: None
// Postconditions: returns a string containing the height in feet(') inches (")
String HeightCalculator::getHeightInFeet() {
	int RemainInches = inches - (floor(feet) * 12);
	int foot = floor(feet);
	String height = "" + to_string(foot) + "' " + to_string(RemainInches) + "\"";
	return height;
}

// getHeightInMeters: gets the calculated in meters
// Preconditions: None
// Postconditions: returns a string containing the height in meters (m)
String HeightCalculator::getHeightInMeters() {
    ostringstream obj;
    obj << fixed;
    obj << setprecision(3);
    obj << meters;

    String height = "" + obj.str() + "m";
    return height;
}

// getFeet: returns height in feet only
double HeightCalculator::getFeet() {
	return feet;
}

// getInches: returns height in inches only
int HeightCalculator::getInches() {
	return inches;
}

// getMeters: returns height in meters
double HeightCalculator::getMeters() {
	return meters;
}

// setPaperDimensions: sets the user input paper dimensions
// Preconditions: all parameter values are positive and in inches
// Postconditions: calls calculateHeight() with newly entered dimensions
void HeightCalculator::setPaperDimensions(double dim1, double dim2) {
	if (dim1 > dim2) {
		inputPaperLong = dim1;
		inputPaperShort = dim2;
	}
	else {
		inputPaperLong = dim2;
		inputPaperShort = dim1;
	}
	calculateHeight();
}

// calculateHeight: calculates the height of the detected person using values passed
// in from the PeopleDetector and the PaperDetector
// Preconditions: all values have been previously set
// Postconditions: sets height in inches, feet, and meters
void HeightCalculator::calculateHeight() {
	if (detectedPaperLong != 0) {
		//find proportion of detected paper's length to the box height, in pixels. 
		//Multiply by 0.9 to adjust for person ID overcompensating the height on average
		double ratio = (boxHeight * 0.9) / detectedPaperLong; 

		//multiply ratio by actual dimensions input by user (in inches)
		double length = ratio * inputPaperLong;
		
		//Adjust box length in inches, based on most common cases
		if (length <= 72.0) {
			length += 1.0;
		}
		else {
			length -= 1.0;
		}

		inches = round(length);
		feet = length / 12;
		meters = length * 0.0254;
	}
}


