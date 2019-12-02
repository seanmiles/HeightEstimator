#include "HeightCalculator.h"

HeightCalculator::HeightCalculator(int dpl, int dps, int bh) {
	feet = 0.0;
	inches = 0;
	meters = 0.0;
	inputPaperLong = 0.0;
	inputPaperShort = 0.0;
	if (dpl > dps) {
		detectedPaperLong = dpl;
		detectedPaperShort = dps;
	}
	else {
		detectedPaperLong = dps;
		detectedPaperShort = dpl;
	}
	boxHeight = bh;
}

HeightCalculator::HeightCalculator(double ipl, double ips, int dpl, int dps, int bh) {
	if (dpl > dps) {
		detectedPaperLong = dpl;
		detectedPaperShort = dps;
	}
	else {
		detectedPaperLong = dps;
		detectedPaperShort = dpl;
	}
	boxHeight = bh;

	setPaperDimensions(ipl, ips);
}

String HeightCalculator::getHeightInFeet() {
	int RemainInches = inches - (floor(feet) * 12);
	String height = "" + to_string(floor(feet)) + "' " + to_string(RemainInches) + "\"";
	return height;
}

String HeightCalculator::getHeightInMeters() {
	String height = "" + to_string(meters) + "m";
	return height;
}

double HeightCalculator::getFeet() {
	return feet;
}

int HeightCalculator::getInches() {
	return inches;
}

double HeightCalculator::getMeters() {
	return meters;
}

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

void HeightCalculator::calculateHeight() {
	//find ratio of detected paper's length to the box height, in pixels
	double ratio = boxHeight / detectedPaperLong;
	
	//multiply ratio by actual dimensions input by user (in inches)
	double length = ratio * inputPaperLong;

	inches = floor(length);
	feet = length / 12;
	meters = length * 0.0254;
}


