// =========================
// YoloObjectDetector Header
// Detect objects. Fast
// =========================


#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <opencv2/opencv.hpp>

#include "network.h"
#include "detection_layer.h"
#include "region_layer.h"
#include "cost_layer.h"
#include "utils.h"
#include "parser.h"
#include "box.h"
#include "image.h"
#include "demo.h"
#include <sys/time.h>


#ifndef RILEY_YOLOOBJECTDETECTOR_H
#define RILEY_YOLOOBJECTDETECTOR_H


class YoloObjectDetector {
public:
    explicit YoloObjectDetector(char*, char*, char*);
    virtual ~YoloObjectDetector();

    // Member functions
    void *detect(char*);
    double *feynman(detection*);

private:
    void *fetch_in_thread();
    void *detect_in_thread();
    void *display_in_thread();

    static int size_network(network*);
    void remember_network(network*);
    detection *avg_predictions(network*, int*);
    
    static void *open_video_stream(char*);
    static image get_image_from_stream(void*);
    static void make_window(char*, int, int);
    
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
};


image mat_to_image(const cv::Mat&);


char *str_to_char_array(const std::string&);
void help();


#endif //RILEY_YOLOOBJECTDETECTOR_H
