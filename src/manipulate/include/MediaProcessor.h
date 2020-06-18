// =====================
// MediaProcessor Header
// Process video data
// =====================


// Includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <random>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

#include "../../opennn/opennn/opennn.h"


#ifndef RILEY_MEDIAPROCESSOR_H
#define RILEY_MEDIAPROCESSOR_H


class MediaProcessor
{
public:
    // Constructor and destructor
    explicit MediaProcessor(const cv::String& name);
    virtual ~MediaProcessor();

    // Methods to process data
    void rgb_stream();
    OpenNN::Matrix<int> fetch_data_matrix();
    void save_frames(const std::string&);
    void resize(const int&, const int&);
    void get_target();

private:
    // Location of the file
    std::string path;

    // Target value
    int target;

    // Parameters
    const int FRAME_HEIGHT = 100;
    const int FRAME_WIDTH = 100;
    const int NUM_FRAMES_FOR_DATA = 5;
    const int RANDOM_NUMBER_BUFFER = 50;
    const int WHEEL_BUFFER = 9;
};


int permute_wheel(int);


#endif //RILEY_MEDIAPROCESSOR_H