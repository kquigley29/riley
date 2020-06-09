// ============================
// Process video data into luma
// ============================


// ===================================================================
// Compile command
// g++ process.cpp -o process.exe `pkg-config opencv4 --cflags --libs`
//====================================================================


// Includes
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>


class VideoProcessor
{
public:
    explicit VideoProcessor(const cv::String &name);
};

