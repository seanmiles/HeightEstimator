#include "PeopleDetector.h"

PeopleDetector::PeopleDetector(Mat newImg) {
	// Set SVM Detector for HoG
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	// Resize to a constant height for easier detection
	float ratio = DETECT_HEIGHT / (float)newImg.rows;
	resize(newImg, img, Size(newImg.cols * ratio, DETECT_HEIGHT));
	// Initialize person's height
	personHeight = 0.0;
}

void PeopleDetector::detectPeople() {
	// Detect in image
	hog.detectMultiScale(img, detections, weights, 0, Size(2, 2), Size(), 1.1, 2, false);
	// Perform non maximum suppression
	vector<float> confidences(weights.begin(), weights.end());
	dnn::NMSBoxes(detections, confidences, 0, 0, indices, 1.2f, 0);
    NMSIndex = indices.front();
	// Calculate height in pixels
	if (detections.size() > 0) {
		calculateHeight();
	}
	else {
		cout << "No person detected. Height will be 0." << endl;
	}
	
}

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
	waitKey(0);
}

void PeopleDetector::calculateHeight() {
	Rect person = detections[NMSIndex];
	Point top = person.tl();
	Point bottom = person.br();
	personHeight = abs(top.y - bottom.y);
}

double PeopleDetector::getHeight() {
	return personHeight;
}

Rect PeopleDetector::getDetectedRect() {
	if (detections.size() > 0) {
		return detections[NMSIndex];
	}
	else {
		Rect base(0, 0, 1, 1);
		return base;
	}
	
}

Mat PeopleDetector::getDetectedImg() {
	return img;
}
