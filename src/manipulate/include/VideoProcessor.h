// =====================
// VideoProcessor Header
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


class VideoProcessor
{
public:
    // Declare constructor and destructor
    explicit VideoProcessor(const cv::String &name);
    virtual ~VideoProcessor();

    // Methods to process data
    void rgb_stream();
    OpenNN::Matrix<double> fetch_data_matrix();

private:
    // File name
    // Location of video
    std::string file_name;
    std::string video_path;

    // Target value
    int target;

    // Parameters
    const int FRAME_HEIGHT = 100;
    const int FRAME_WIDTH = 100;
    const int NUM_FRAMES_FOR_DATA = 5;
    const int RANDOM_NUMBER_BUFFER = 50;
};

