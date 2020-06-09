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
    cv::String data_path = "/home/riley/Projects/riley/data/store/" + name + ".txt";

    cv::VideoCapture capture(video_path);

    cv::Mat frame;
    bool okay;
    okay = capture.read(frame);

    int frame_count = 0;

    std::ofstream data;
    data.open(data_path);

    while (okay && frame_count < 20) {

        std::cout << "Frame: " << frame_count << std::endl;

        for (int i = 0; i < frame.rows; i++)
        {
            const cv::Vec3b *Mi = frame.ptr<cv::Vec3b>(i);

            for (int j = 0; j < frame.cols; j++)
            {
                cv::Vec3b pixel = Mi[j];

                double luma = 0.2126 * pixel[0] + 0.7152 * pixel[1] + 0.0722 * pixel[2];
                data << luma << ",";
            }
        }

        frame_count++;
        okay = capture.read(frame);

    }
    data.close();

}

