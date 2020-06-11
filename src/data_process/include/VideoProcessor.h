// ============================
// VideoProcessor Header
// Process video data into luma
// ============================


// Includes
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>


class VideoProcessor
{
public:
    // Declare constructor and destructor
    explicit VideoProcessor(const cv::String &name);
    virtual ~VideoProcessor();

    // Methods to retrieve and return the luma data
    void process();
    std::vector<double> get_luma();

private:
    // Store the luma data from a video
    std::vector<double> luma_data {};

    // File name
    // Location of video and data
    std::string file_name;
    std::string video_path;
    std::string data_path;
};

