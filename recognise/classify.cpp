#include <opencv2/opencv.hpp>
#include <iostream>

#define RASPBERRY_PI

#ifdef RASPBERRY_PI
#include "RaspiCamCV.h"
#endif

int main(int argc, char** argv) {
	std::vector<cv::Rect> faces;
	std::string face_cascade_name = "../cascade/lbpcascade_frontalface.xml";

    // Comment lbph_model and uncomment fisher_model to use fishers instead of lbp.
	std::string classifier_file_name = "../classifiers/lbph_model.yaml";
	//std::string classifier_file_name = "../classifiers/fisher_model.yaml";

	cv::CascadeClassifier face_cascade;
	cv::Ptr<cv::FaceRecognizer> model = cv::createLBPHFaceRecognizer();
	//cv::Ptr<cv::FaceRecognizer> model = cv::createFisherFaceRecognizer();

#ifndef RASPBERRY_PI
	cv::VideoCapture cap;
    cap.open(0);
	if(!cap.isOpened()) {
		std::cerr << "Unable to open capture device for reading" << std::endl;
        return -1;
	}
#else
    RaspiCamCVCapture* cap;
    cap = raspiCamCvCreateCameraCapture(0);
    if(cap == NULL){
        std::cerr << "Unable to open capture device for reading" << std::endl;
        return -1;
    }
#endif
	//cap.open("Face.mp4");
	
	if(!face_cascade.load(face_cascade_name)) {
		std::cerr << "Unable to load cascade classifier" << std::endl;
		return -1;
	}
	
    model->load(classifier_file_name);

	while(1) {
		cv::Mat gray_frame;
#ifndef RASPBERRY_PI
        cv::Mat frame;
		cap >> frame;
#else
        IplImage* timage = raspiCamCvQueryFrame(cap);
        cv::Mat frame(timage);
#endif
		if(!frame.data ) {
			std::cerr << "Could not read from capture device" << std::endl ;
			return -1;
		}
		cvtColor(frame, gray_frame, CV_BGR2GRAY);
		face_cascade.detectMultiScale(gray_frame, faces, 1.1, 3, 0, cv::Size(24, 24));
		for(int i=0; i < faces.size(); i++) {
			cv::Rect face = faces[i];
			cv::Mat roi = gray_frame(face);
			cv::Mat resized_face = cv::Mat::zeros(cv::Size(250,250), CV_8UC1);
			resize(roi, resized_face, cv::Size(250,250), 0, 0, CV_INTER_CUBIC);

			int label=-1;
			double confidence=0;
			model->predict(resized_face, label, confidence);
            std::cout << label << " " << confidence << std::endl;

            // Here you can add your confidence threshold
            // Ex: if(label > 0 && confidence < threshold)
			if(label > 0)
				cv::rectangle(frame, cv::Point(face.x, face.y), 
					cv::Point(face.x+face.width, face.y+face.height),
					cv::Scalar(0,255,0), 2);
			else 
				cv::rectangle(frame, cv::Point(face.x, face.y), 
					cv::Point(face.x+face.width, face.y+face.height),
					cv::Scalar(0,0,255), 2);
		}
		cv::imshow("camera stream", frame);
		int key_press = cv::waitKey(30);
		if(key_press == 27)
			break;
	}
    raspiCamCvReleaseCapture(cap);
    return 0;
}