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
    explicit ObjectDetector() = default;
    explicit ObjectDetector(char *data_cfg, char *cfg, char *weights, const int &tracker_limit=50);
    explicit ObjectDetector(const int &index, char *data_cfg, char *cfg, char *weights, const int &tracker_limit=50);
    explicit ObjectDetector(const char *video,char *data_cfg, char *cfg, char *weights,  const int &tracker_limit=50);
    virtual ~ObjectDetector();

    void detect();
    void detect(cv::Mat image);
    void display();

    cv::Mat get_img() const;
    detection *get_dets() const;
    int get_nboxes() const;

    void set_img(const cv::Mat &image);
    void set_tracking(const bool &track);

    void update_detect_thresh(const float &new_thresh);

private:
    char **detect_labels;
    image **detect_alphabet;
    cv::VideoCapture *cap;
    network *net;
    float detect_thresh = 0.5;
    float detect_hier = 0.5;
    float detect_nms = 0.45;
    int nboxes = 0;
    detection *dets;
    layer l;
    image im;
    cv::Mat img;
    bool track = false;
    CentroidTracker *tracker;
};


#endif //RILEY_OBJECTDETECTOR_H
