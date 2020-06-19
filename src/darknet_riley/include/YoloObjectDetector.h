// =========================
// YoloObjectDetector Header
// Detect objects. Fast
// =========================


#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>


extern "C" {
#include "darknet.h"
}

extern "C" image mat_to_image(cv::Mat);
extern "C" cv::Mat image_to_mat(image);


#ifndef RILEY_YOLOOBJECTDETECTOR_H
#define RILEY_YOLOOBJECTDETECTOR_H


class YoloObjectDetector {
public:
    explicit YoloObjectDetector(char*, char*, char*); 
    virtual ~YoloObjectDetector();

    // Member functions
    detection *detect(char*);

private:
    network *net;

    image buff_[3];
    image buff_letter_[3];
    int buff_index_ = 0;
    void *cap;
    float FPS_ = 0;
    const float THRESHOLD_ = 0.5;
    const float HIER_ = 0.5;
    int RUNNING_ = 0;

    int DETECT_FRAME_ = 3;
    int DETECT_INDEX_ = 0;
    float **predictions;
    float *avg;
    int DETECT_DONE_ = 0;
    int DETECT_TOTAL_ = 0;

    int CLASSES = 3;
    char** NAMES;
};


#endif //RILEY_YOLOOBJECTDETECTOR_H
