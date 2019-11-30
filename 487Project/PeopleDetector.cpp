#include "PeopleDetector.h"

PeopleDetector::PeopleDetector(Mat newImg)
{
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    img = newImg;
}

void PeopleDetector::detectPeople()
{
    // Detect in image
    hog.detectMultiScale(img, detections, 0, Size(8, 8), Size(32, 32), 1.2, 2);
}

void PeopleDetector::displayPeople()
{
    // Draw boxes around detections
    for (auto& detection : detections)
    {
        rectangle(img, detection.tl(), detection.br(), Scalar(0, 0, 255), 2);
    }

    // Display detections
    namedWindow("Detected People");
    imshow("Detected People", img);
    imwrite("people_out.jpg", img);
    waitKey(0);
}