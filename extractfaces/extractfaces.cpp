#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
	cv::Mat image;
	std::vector<cv::Rect> faces;

    image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
    if(! image.data ) {
        std::cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    std::string face_cascade_name = "../cascade/lbpcascade_frontalface.xml";
	cv::CascadeClassifier face_cascade;
	if(!face_cascade.load(face_cascade_name)) {
		std::cerr << "Unable to load cascade classifier" << std::endl;
		return -1;
	}
    face_cascade.detectMultiScale(image, faces, 1.1, 5, 0, cv::Size(24, 24));

    int i = faces.size() - 1;
    cv::imwrite(argv[2], image(faces[i]));

    return 0;
}