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
    Mat img;

public:
    PeopleDetector(Mat newImg);
    void detectPeople();
    void displayPeople();

};

#endif