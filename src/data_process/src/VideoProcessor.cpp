// ============================
// VideoProcessor Source File
// Process video data into luma
// ============================


// Includes
#include "../include/VideoProcessor.h"


// VideoProcessor constructor
VideoProcessor::VideoProcessor(const cv::String& name)
{
    // Get the video and data file locations and names
    cv::String video_path = "/home/riley/Projects/riley/data/videos/" + name + ".mp4";
    cv::String data_path = "/home/riley/Projects/riley/data/store/" + name + ".txt";

    // Capture the video
    cv::VideoCapture capture(video_path);

    // Declare the frame matrix and boolean of if the frame exists
    // Retrieve the first frame
    // declare frame count and string stream to store the luma values
    cv::Mat frame;
    bool okay;
    okay = capture.read(frame);
    int frame_count = 0;
    std::stringstream lumas;

    // While there is a frame in the video
    while (okay) {

        // Print the frame count
        std::cout << "Frame: " << frame_count << "\n";

        // For every pixel in the frame
        for (int i = 0; i < frame.rows; i++)
        {
            const cv::Vec3b *Mi = frame.ptr<cv::Vec3b>(i);

            for (int j = 0; j < frame.cols; j++)
            {
                // Get the pixels RGB values and calculate the luma
                // Normalise the luma and round to 4 decimal places
                cv::Vec3b pixel = Mi[j];
                double luma = 0.2126 * pixel[0] + 0.7152 * pixel[1] + 0.0722 * pixel[2];
                luma = round(luma * 10000.0) / 10000.0;

                // Add luma value to lumas string stream
                lumas << luma << ",";

            }
        }
        // Add to frame count and get the next frame
        frame_count++;
        okay = capture.read(frame);

    }
    // Add data to the specified file
    std::ofstream data;
    data.open(data_path);
    data << lumas.str();
    data.close();

}

