// ============================
// VideoProcessor Source
// Process video data into luma
// ============================


// Includes
#include "../include/VideoProcessor.h"


// VideoProcessor constructor
VideoProcessor::VideoProcessor(const cv::String& name)
{
    // Get the file name
    file_name = name;
    video_path = "/home/riley/Projects/riley/data/videos/" + name + ".mp4";
    data_path = "/home/riley/Projects/riley/data/store/" + name + ".txt";
}


// Get the luma values from the video and
// store in the luma_data vector
void VideoProcessor::process() {
    // Capture the video
    cv::VideoCapture capture(video_path);

    // Declare the frame matrix and boolean of if the frame exists
    // Retrieve the first frame
    // declare frame count
    cv::Mat frame;
    bool okay;
    okay = capture.read(frame);
    int frame_count = 0;

    // Declare file stream var and begin writing to the file
    std::ofstream data;
    data.open(data_path);
    data << "luma = [";
    data.close();

    // While there is a frame in the video
    while (okay) {

        // Print the frame count
        std::cout << "Frame: " << frame_count << "\n";

        // Frame data vector
        std::vector<double> frame_data{};

        // For every pixel in the frame
        for (int i = 0; i < frame.rows; i++) {
            const cv::Vec3b *Mi = frame.ptr<cv::Vec3b>(i);

            for (int j = 0; j < frame.cols; j++) {
                // Get the pixels RGB values and calculate the luma
                // Round to 4 decimal places
                cv::Vec3b pixel = Mi[j];
                double luma = 0.2126 * pixel[0] + 0.7152 * pixel[1] + 0.0722 * pixel[2];
                luma = round(luma * 1000.0) / 1000.0;

                // Add luma value to lumas_data vector
                frame_data.push_back(luma);

            }
        }

        // Push back the frame data into video's luma data
//        luma_data.push_back(frame_data);

        // Write the frames data to the file
        data.open(data_path);
        data << "[";
        for (int i = 0; i < frame_data.size() - 1; i++) data << frame_data[i] << ",";
        data << frame_data[frame_data.size() - 1] << "]";
        data.close();

        // Add to frame count and get the next frame
        frame_count++;
        okay = capture.read(frame);

        // If there is a new frame we can add a comma to the data file
        if (okay)
        {
            data.open(data_path);
            data << ",";
            data.close();
        }

    }

    // End off the data file list format
    data.open(data_path);
    data << "]";
    data.close();
}


// Returns luma vector
        std::vector<std::vector<double>> VideoProcessor::get_luma()
{
    return luma_data;
}


// Define a trivial destructor with 'default'
VideoProcessor::~VideoProcessor() = default;
