// =========================
// YoloObjectDetector Header
// Detect objects. Fast
// =========================


#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "darknet.h"


extern "C" {
#include "network.h"
}


#ifndef RILEY_YOLOOBJECTDETECTOR_H
#define RILEY_YOLOOBJECTDETECTOR_H


class YoloObjectDetector {
public:
    YoloObjectDetector();
    ~YoloObjectDetector();
};


#endif //RILEY_YOLOOBJECTDETECTOR_H
