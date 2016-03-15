#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
	while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
	std::cout << path << std::endl;
        if(!path.empty() && !classlabel.empty()) {
			Mat temp = imread(path, 0);
			Mat rtemp = Mat::zeros(Size(250,250), CV_8UC1);
			resize(temp, rtemp, Size(250,250), 0, 0, INTER_CUBIC);
			if(rtemp.data) {
				images.push_back(rtemp);
				labels.push_back(atoi(classlabel.c_str()));
			}
        }
		liness.clear();
    }
	file.close();
}


int main(int argc, const char *argv[]) {
    // Check for valid command line arguments, print usage
    // if no arguments were given.
    if (argc < 2) {
        cout << "usage: " << argv[0] << " <csv.ext> <output_folder> " << endl;
        exit(1);
    }
    
    // Get the path to your CSV.
    //string fn_csv = string(argv[1]);
    // These vectors hold the images and corresponding labels.
    vector<Mat> images;
    vector<int> labels;
    // Read in the data. This can fail if no valid
    // input filename is given.
    try {
        read_csv(argv[1], images, labels);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << argv[1] << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(-1);
    }
    
    Ptr<FaceRecognizer> fisher_model = createFisherFaceRecognizer();
    fisher_model->train(images, labels);
    std::string filename;
    filename = argv[2]+std::string("/")+std::string("fisher_model.yaml");
    fisher_model->save(filename);

    Ptr<FaceRecognizer> lbph_model = createLBPHFaceRecognizer();
    lbph_model->train(images, labels);
    filename = argv[2]+std::string("/")+std::string("lbph_model.yaml");
    lbph_model->save(filename);

    return 0;
}
