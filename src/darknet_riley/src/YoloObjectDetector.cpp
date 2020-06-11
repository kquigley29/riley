// ================================
// Yolo Object Detector Source File
// ================================


// Includes
#include "../include/YoloObjectDetector.h"


// YoloObjectDetector Constructor
YoloObjectDetector::YoloObjectDetector(const std::string& name, const std::string& weights, const std::string& cfg)
{
    // Set up the paths to the files
    video_name = name;
    video_path += name;
    weights_model = weights;
    weights_path += weights;
    cfg_model = cfg;
    cfg_path += cfg;


    std::cout << "I spy with my little eye..." << "\n"
              << "something beginning with..." << "\n"
              << "\n"
              << "M        M" << "\n"
              << "M M    M M" << "\n"
              << "M   MM   M" << "\n"
              << "M        M" << "\n"
              << "M        M  o n e y" << "\n";
}

void YoloObjectDetector::detect()
{
    cv::VideoCapture capture(video_path);

    bool okay;
    cv::Mat frame;
    okay = capture.read(frame);

}


// Default destructor
YoloObjectDetector::~YoloObjectDetector() = default;
