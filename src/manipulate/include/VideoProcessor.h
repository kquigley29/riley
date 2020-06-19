// =====================
// VideoProcessor Header
// Process video data
// =====================


// Includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <random>
#include <cmath>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include "opennn.h"


#ifndef RILEY_VIDEOPROCESSOR_H
#define RILEY_VIDEOPROCESSOR_H


class VideoProcessor {
public:
    explicit VideoProcessor(const std::string&);
    virtual ~VideoProcessor();

    // Member functions
    std::string get_path();
    void set_path();
    OpenNN::Matrix<double_t> get_random_data_matrix();
    void collect_frames();


private:
    std::string path;
    int target;

    // Params
    int VIDEO_FRAME_WIDTH_ = 100;
    int VIDEO_FRAME_HEIGHT_ = 100;
    int NUM_FRAMES_OF_DATA_ = 5;
    int RANDOM_NUMBER_BUFFER_ = 50;
    int NUM_WHEEL_POCKETS_ = 37;
    int TARGET_INCLUSION_BUFFER_ = 9;

    // Wheel pocket numbering
    const std::vector<int> wheel = {0, 32, 15, 19, 4, 21, 2, 25, 17, 34, 6, 27, 13, 36, 11, 30, 8, 23, 10, 5, 24, 16, 33, 1, 20, 14, 31, 9, 22, 18, 29, 7, 28, 12, 35, 3, 26};
};


int permute_wheel(int);


#endif //RILEY_VIDEOPROCESSOR_H