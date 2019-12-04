#include "PeopleDetector.h"

PeopleDetector::PeopleDetector(Mat newImg)
{
    // Set SVM Detector for HoG
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    // Resize to a constant height for easier detection
    float ratio = DETECT_HEIGHT / (float)newImg.rows;
    resize(newImg, img, Size(newImg.cols * ratio, DETECT_HEIGHT));
    // Initialize person's height
    personHeight = 0.0;
}

void PeopleDetector::detectPeople()
{
    // Detect in image
    hog.detectMultiScale(img, detections, weights, 0, Size(4, 4), Size(), 1.1, 2, false);
    // Perform non maximum suppression
    vector<float> confidences(weights.begin(), weights.end());
    dnn::NMSBoxes(detections, confidences, 0, 0, indices, 1.2f, 0);
    // Calculate height in pixels
    calculateHeight();
}

void PeopleDetector::displayPeople()
{
    // Slightly shrink height of rectangles and draw
    for (int index: indices)
    {
        Rect r = detections[index];

        /*r.x += cvRound(r.width * 0.5);
        r.x += -40;
        r.width = cvRound(r.width * 0.3);
        r.y += cvRound(r.height * 0.07);
        r.height = cvRound(r.height * 0.8);*/
        rectangle(img, r.tl(), r.br(), cv::Scalar(0, 0, 255), 3);
    }

    // Display detections
    namedWindow("Detected People");
    imshow("Detected People", img);
    imwrite("people_out.jpg", img);
    waitKey(0);
}

void PeopleDetector::calculateHeight() 
{
    Rect person = detections[0];
    Point top = person.tl();
    Point bottom = person.br();
    personHeight = abs(top.y - bottom.y);
}

double PeopleDetector::getHeight() 
{
	//return personHeight * 3;
    return personHeight / img.rows;
}
