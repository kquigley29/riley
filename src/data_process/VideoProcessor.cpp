// ============================
// VideoProcessor Source File
// Process video data into luma
// ============================


// Includes
#include "VideoProcessor.h"


// VideoProcessor constructor
VideoProcessor::VideoProcessor(const cv::String& name)
{

    cv::String video_path = "/home/riley/Projects/riley/data/videos/" + name + ".mp4";

    cv::VideoCapture capture(video_path);

    cv::Mat frame;
    bool okay;
    okay = capture.read(frame);

    int frame_count = 0;

    while (okay) {
        frame_count++;

        std::cout << "Frame: " << frame_count << std::endl;

        for (int i = 0; i < frame.rows; i++) {
            const cv::Vec3b *Mi = frame.ptr<cv::Vec3b>(i);

            for (int j = 0; j < frame.cols; j++) {
                cv::Vec3b pixel = Mi[j];

                double luma = 0.2126 * pixel[0] + 0.7152 * pixel[1] + 0.0722 * pixel[2];

//                std::cout << "Luma of "
//                          << "[" << frame_count << ", " << i << ", " << j << "]: "
//                          << luma << std::endl;
            }
        }

        okay = capture.read(frame);

    }

}

