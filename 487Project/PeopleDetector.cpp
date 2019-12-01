#include "PeopleDetector.h"

PeopleDetector::PeopleDetector(Mat newImg)
{
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    img = newImg;
}

void PeopleDetector::detectPeople()
{
    // Detect in image
    hog.detectMultiScale(img, detections, 0, Size(3, 3), Size(), 1.05, 2, false);

    // Eliminate overlapping rectangles
    int i, j;
    for (i = 0; i < detections.size(); i++)
    {
        Rect r = detections[i];
        for (j = 0; j < detections.size(); j++) 
        {
            if (j != i) {
                Rect iRect = r;
                Rect jRect = detections[j];
                Rect intersectRect = (iRect & jRect);
                if (intersectRect.area() >= iRect.area() * 0.9) break;
            }
        }
        if (j == detections.size())
            detections_filtered.push_back(r);
    }


}

void PeopleDetector::displayPeople()
{
    // Slightly shrink height of rectangles and draw
    for (int i = 0; i < detections_filtered.size(); i++)
    {
        Rect r = detections_filtered[i];

        r.x += cvRound(r.width * 0.5);
        r.x += -40;
        r.width = cvRound(r.width * 0.3);
        r.y += cvRound(r.height * 0.07);
        r.height = cvRound(r.height * 0.8);
        rectangle(img, r.tl(), r.br(), cv::Scalar(0, 0, 255), 3);
    }

    // Display detections
    namedWindow("Detected People");
    imshow("Detected People", img);
    imwrite("people_out.jpg", img);
    waitKey(0);
}