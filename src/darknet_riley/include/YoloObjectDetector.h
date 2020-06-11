// ============================
// YoloObjectDetector Header
// Detect balls and green spots
// ============================


// Includes
#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include "../../darknet/include/darknet.h"


#ifndef YOLO_OBJECT_DETECTOR_H
#define YOLO_OBJECT_DETECTOR_H


class YoloObjectDetector {

public:
    // Constructor and destructor
    explicit YoloObjectDetector(const std::string&, const std::string&, const std::string&);
    virtual ~YoloObjectDetector();

    // Member functions
    // Detect the desired objects in the video
    void detect();

private:
    // Path and names of required files
    std::string video_name;
    std::string weights_model;
    std::string cfg_model;
    std::string video_path {"/home/riley/Projects/riley/data/videos/"};
    std::string weights_path {"/home/riley/Projects/riley/src/darknet_riley/weights/"};
    std::string cfg_path {"home/riley/Projects/riley/src/darknet_riley/cfg/"};
};


#endif //YOLO_OBJECT_DETECTOR_H
