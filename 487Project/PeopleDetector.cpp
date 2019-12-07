//34567891*********2*********3*********4*********5*********6*********7*********8
// ------------------------- PeopleDetector.cpp --------------------------------
// Tung Dinh, Sean Miles, & Bradley Pratt CSS 487
// Created: 11/21/19
// Last Edit: 12/6/19
// -----------------------------------------------------------------------------
// Purpose - Utilizes opencv methods to identify a single person in an image 
// input by user. Displays a rectangle around the person identified.
// -----------------------------------------------------------------------------
// Assumptions - Input image is a valid image file. 
// -----------------------------------------------------------------------------

#include "PeopleDetector.h"

// PeopleDetector: constructor for PeopleDetector object
// Preconditions: newIMg is a valid image
// Postconditions: creates new PeopleDetector
PeopleDetector::PeopleDetector(Mat newImg) {
	// Set SVM Detector for HoG
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	// Resize to a constant height for easier detection
	float ratio = DETECT_HEIGHT / (float)newImg.rows;
	resize(newImg, img, Size(newImg.cols * ratio, DETECT_HEIGHT));

	// Initialize person's height
	personHeight = 0.0;
	NMSIndex = 0;
}

// detectPeople: detects people in image and uses NMS to determine create 
// strongest case
// Preconditions: None
// Postconditions: detections contains rectangle outlining detected person
void PeopleDetector::detectPeople() {
	// Detect in image
	hog.detectMultiScale(img, detections, weights, 0, Size(2, 2), 
		Size(), 1.1, 2, false);

	// Perform non maximum suppression
	vector<float> confidences(weights.begin(), weights.end());
	dnn::NMSBoxes(detections, confidences, 0, 0, indices, 1.2f, 0);

    // Get first index after non maximum suppression
    if (indices.size() > 0) {
        NMSIndex = indices.front();
    }
    
	// Calculate height in pixels
	if (detections.size() > 0) { //if detections are found in input img
		calculateHeight();
	}
	else {
		cout << "No person detected. Height will be 0." << endl;
	}
}

// displayPeople: draws Rectangles outlining detected person to img
// Preconditions: None
// Postconditions: Displays the img with person boxed in to console
void PeopleDetector::displayPeople() {
	if (detections.size() > 0) {
		// Slightly shrink height of rectangles and draw
		for (int index : indices) {
			Rect r = detections[index];

			// Decrease rectangle area slightly due to common overestimation
			r.x += cvRound(r.width * 0.15);
			r.width = cvRound(r.width * 0.7);
			r.y += cvRound(r.height * 0.025);
			r.height = cvRound(r.height * 0.9);
			rectangle(img, r.tl(), r.br(), cv::Scalar(0, 0, 255), 3);
		}
	}

	// Display detections
	namedWindow("Detected People");
	imshow("Detected People", img);
	imwrite("people_out.jpg", img);
}

// calculateHeight: calculates height of the identification box
// Preconditions: detections is not empty (person has been ID'd)
// Postconditions: personHeight contains the height of the ID box
void PeopleDetector::calculateHeight() {
	Rect person = detections[NMSIndex]; //strongest case
	Point top = person.tl();
	Point bottom = person.br();
	personHeight = abs(top.y - bottom.y);
}

// getHeight: returns personHeight
double PeopleDetector::getHeight() {
	return personHeight;
}

// getDetectedRect: returns ID box in form of Rect if one is found,
// otherwise returns a 1 x 1 Rect at the origin (height will be zero
// regardless in this scenario)
Rect PeopleDetector::getDetectedRect() {
	if (detections.size() > 0) {
		return detections[NMSIndex];
	}
	else {
		Rect base(0, 0, 1, 1);
		return base;
	}
}

// getDetectedImg: returns img used for person detection
Mat PeopleDetector::getDetectedImg() {
	return img;
}
