#ifndef PEOPLEDETECTOR_H
#define PEOPLEDETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
using namespace std;

class PeopleDetector
{

private:
    HOGDescriptor hog;
    vector<Rect> detections;
    vector<double> confidences;
    Mat img;

public:
    PeopleDetector(Mat newImg);
    void detectPeople();
    void displayPeople();

};

#endif