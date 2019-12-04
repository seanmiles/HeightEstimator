#ifndef PEOPLEDETECTOR_H
#define PEOPLEDETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
using namespace std;

class PeopleDetector
{

private:
    const int DETECT_HEIGHT = 400;
    HOGDescriptor hog;
    vector<Rect> detections;
    vector<double> weights;
    vector<int> indices;
    Mat img;
	double personHeight;            // store height of box
	void calculateHeight();      // calculates height of box outlining person detection

public:
    PeopleDetector(Mat newImg);
    void detectPeople();
    void displayPeople();

	double getHeight(); // returns pixel height of person in image
	Rect getDetectedRect();
	Mat getDetectedImg();
};

#endif