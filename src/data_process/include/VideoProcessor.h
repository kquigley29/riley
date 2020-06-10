// ============================
// VideoProcessor Header File
// Process video data into luma
// ============================


// Includes
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>


class VideoProcessor
{
public:
    explicit VideoProcessor(const cv::String &name);
};

