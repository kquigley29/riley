// =========================
// ObjectDetector Header
// Detect objects. Fast
// =========================


#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <opencv2/opencv.hpp>
#include <darknet.h>
#include <demo.h>
#include "CentroidTracker.h"
#include "RileyUtils.h"



#ifndef RILEY_YOLOOBJECTDETECTOR_H
#define RILEY_YOLOOBJECTDETECTOR_H


class ObjectDetector {
public:
    explicit ObjectDetector(char *data_cfg, char *cfg, char *weights, const int &tracker_limit=50);
    virtual ~ObjectDetector();

    void *detect(const char *video, const int &index);

private:
    void *fetch_in_thread();
    void *detect_in_thread();
    void *display_in_thread();

    void remember_network(network *net);
    detection *avg_predictions(network *net, int *nboxes);

    int detect_classes;
    char **detect_labels;
    image **detect_alphabet;

    network *net;
    int detect_total;
    int detect_frame = 3;
    double detect_time;
    int detect_index = 0;
    int detect_done = 0;
    float *avg;
    void *cap;
    float **predictions;
    float detect_thresh = 0.5;
    float detect_hier = 0.5;
    image buff[3];
    image buff_letter[3];
    int buff_index = 0;
    float fps = 0;
    int running = 0;

    CentroidTracker *tracker;
};



void help();


#endif //RILEY_YOLOOBJECTDETECTOR_H
