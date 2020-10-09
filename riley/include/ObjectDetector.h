// =========================
// ObjectDetector Header
// Detect objects. Fast
// =========================


#ifndef RILEY_OBJECTDETECTOR_H
#define RILEY_OBJECTDETECTOR_H


#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <opencv2/opencv.hpp>
#include <darknet.h>
#include <demo.h>
#include "CentroidTracker.h"
#include "riley_utils.h"


class ObjectDetector {
public:
    explicit ObjectDetector(char *data_cfg, char *cfg, char *weights, const int &index, const int &tracker_limit=50);
    explicit ObjectDetector(char *data_cfg, char *cfg, char *weights, const char *video, const int &tracker_limit=50);
    virtual ~ObjectDetector();

    void *detect(const bool &track=false);

private:
    cv::VideoCapture *get_cap(const int &index);
    cv::VideoCapture *get_cap(const char *video);

    int detect_classes;
    char **detect_labels;
    image **detect_alphabet;

    network *net;
    int detect_total;
    int detect_frame = 3;
    float *avg;
    void *cap;
    float **predictions;
    float detect_thresh = 0.5;
    float detect_hier = 0.5;
    float detect_nms = 0.45;

    CentroidTracker *tracker;
};


#endif //RILEY_OBJECTDETECTOR_H
