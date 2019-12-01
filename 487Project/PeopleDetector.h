#ifndef PEOPLEDETECTOR_H
#define PEOPLEDETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace std;

class PeopleDetector
{

private:
    HOGDescriptor hog;
    vector<Rect> detections;
    vector<Rect> detections_filtered;
    Mat img;
	int personHeight; //store height of box
	void height(); //calculates height of box outlining person detection

public:
    PeopleDetector(Mat newImg);
    void detectPeople();
    void displayPeople();

	int getHeight(); //returns pixel height of person in image
};

#endif