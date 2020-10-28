// =========================
// ObjectDetector Header
// Detect objects. Fast
// =========================


#ifndef RILEY_OBJECTDETECTOR_H
#define RILEY_OBJECTDETECTOR_H


#include <opencv2/opencv.hpp>
#include <darknet.h>
#include "CentroidTracker.h"


class ObjectDetector {
public:
    explicit ObjectDetector(char *data_cfg, char *cfg, char *weights, const unsigned int tracker_limit=50);
    virtual ~ObjectDetector();

    cv::Mat detect(const cv::Mat &img);
    void detect_from_cap(cv::VideoCapture &cap);
    static void display(const cv::Mat &img);

    detection *get_dets() const;
    int get_nboxes() const;

    void set_tracking(const bool &track);
    void set_thresh(const float &new_thresh);

private:
    char **detect_labels;
    image **detect_alphabet;
    network *net;
    float detect_thresh = 0.5;
    float detect_hier = 0.5;
    float detect_nms = 0.45;
    int nboxes = 0;
    detection *dets;
    layer l;
    bool track = false;
    CentroidTracker *tracker;
};


#endif //RILEY_OBJECTDETECTOR_H
