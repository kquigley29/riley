// =====================
// VideoProcessor Source
// Process video data
// =====================


// Includes
#include "../include/VideoProcessor.h"


// VideoProcessor constructor
VideoProcessor::VideoProcessor(const cv::String& name) {

    std::cout << "<<LOAD IT UP>>\n";

    // Get the video file name
    video_path = name;

    std::cout << "VideoProcessor object created with video_path=" << video_path << "\n";

    // Get the target value from the video name
    std::string target_string = name.substr(18, 2);
    target = std::stoi(target_string);

    std::cout << "Target: " << target << "\n";
}


// Process video into 300x300 sized RGB frames
void VideoProcessor::rgb_stream() {
    // Capture the video
    cv::VideoCapture capture(video_path);

    // Declare the frame matrix
    // declare new frame size and frame count
    cv::Mat frame;
    cv::Size frame_size(FRAME_WIDTH,FRAME_HEIGHT);
    int frame_count = 0;

    // While there is a frame in the video
    // Assign the frame matrix values and execute the while block
    while (capture.read(frame)) {

        // Count the frame
        frame_count++;

        // Info
        std::cout << "Frame: " << frame_count << "\n";
        std::cout << "Orig frame size : " << frame.rows << " x " << frame.cols << "\n";

        // Resize the frame
        cv::resize(frame, frame, frame_size);
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

        // Info
        std::cout << "New frame size: " << frame.rows << " x " << frame.cols << "\n";

        // For every pixel in the frame
        for (int i = 0; i < frame.rows; i++) {

            const cv::Vec3b *Mi = frame.ptr<cv::Vec3b>(i);

            for (int j = 0; j < frame.cols; j++) {

                cv::Vec3b pixel = Mi[j];
                double red = pixel[0];
                double green = pixel[1];
                double blue = pixel[2];
                std::cout << "R: " << red << "\n" <<
                             "G: " << green << "\n" <<
                             "B: " << blue << "\n";
           }
        }
    }
}


OpenNN::Matrix<double> VideoProcessor::fetch_data_matrix() {

    // Declare the target vector and populate it
    OpenNN::Vector<double> target_vector(37);
    for (int i = 0; i < target_vector.size(); i++) {
        if (i != target) target_vector[i] = 0;
        else if (i == target) target_vector[i] = 1;
    }

    // Capture the video
    cv::VideoCapture capture(video_path);

    // Skip the unwanted frames
    int num_frames = capture.get(cv::CAP_PROP_FRAME_COUNT);
    std::random_device random_number;
    int random_frame = random_number() % (num_frames - RANDOM_NUMBER_BUFFER);
    int read_count = 0;
    cv::Mat unwanted_frame;
    while (read_count < random_frame) {
        capture.read(unwanted_frame);
        read_count++;
    }

    // Declare the frame to be used and the frame size
    // Declare colour indices for data vector
    // Loop NUM_FRAMES_FOR_DATA times to get data from frames
    OpenNN::Matrix<double> data_matrix(NUM_FRAMES_FOR_DATA * 3, FRAME_HEIGHT * FRAME_WIDTH + target_vector.size());
    cv::Mat frame;
    int red_index = 0;
    int green_index = 1;
    int blue_index = 2;
    cv::Size frame_size(FRAME_WIDTH, FRAME_HEIGHT);
    for (int i = 0; i < NUM_FRAMES_FOR_DATA; i++) {
        // Assign values to the image matrix
        // Resize and convert to RGB
        capture.read(frame);
        cv::resize(frame, frame, frame_size);
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

        // Declare data_matrix to store RGB with the correct size
        // Declare vectors to store RGB values
        // Declare pixel_count to count the pixels
        OpenNN::Vector<double> red_vec(FRAME_WIDTH * FRAME_HEIGHT), green_vec(FRAME_WIDTH * FRAME_HEIGHT), blue_vec(FRAME_WIDTH * FRAME_HEIGHT);
        int pixel_count = 0;

        // Loop through the frame's pixels
        // Assign RGB values to their respective vectors
        for (int j = 0; j < frame.rows; j++) {
            const cv::Vec3b *Mi = frame.ptr<cv::Vec3b>(j);

            for (int k = 0; k < frame.cols; k++) {

                cv::Vec3b pixel = Mi[k];

                // Assign the pixel values to the colour
                double red = pixel[0];
                double green = pixel[1];
                double blue = pixel[2];

                // Insert the pixel values into the vectors
                // Increment the pixel count
                red_vec[pixel_count] = red;
                green_vec[pixel_count] = green;
                blue_vec[pixel_count] = blue;
                pixel_count++;
            }
        }

        // Add the vectors to the matrix
        // Increment the colour indices
        for (int r = 0; r < target_vector.size() + red_vec.size(); r++) {
            if (r > 36) data_matrix(red_index, r) = red_vec[r];
            else data_matrix(red_index, r) = target_vector[r];
        }
        for (int g = 0; g < target_vector.size() + green_vec.size(); g++) {
            if (g > 36) data_matrix(green_index, g) = green_vec[g];
            else data_matrix(green_index, g) = target_vector[g];
        }
        for (int b = 0; b < target_vector.size() + blue_vec.size(); b++) {
            if (b > 36) data_matrix(blue_index, b) = blue_vec[b];
            else data_matrix(blue_index, b) = target_vector[b];
        }
        red_index += 3;
        green_index += 3;
        blue_index += 3;
    }

    // Info
    std::cout << data_matrix(3,45);
    return data_matrix;
}


void VideoProcessor::save_frames(const std::string& image_name) {

    cv::VideoCapture capture(video_path);
    int count = 0;
    cv::Mat frame;
    while (capture.read(frame)) {

        std::string file = "/home/keane/Projects/riley/data/test_images/" + to_string(count) + image_name;
        cv::imwrite(file, frame);
        count++;
    }

    std::cout << "Finished!\n";
}

void VideoProcessor::resize(const int &width, const int &height) {

    cv::Mat image;
    image = cv::imread(video_path);
    cv::Size image_size(width, height);
    cv::resize(image, image, image_size);

    cv::imwrite(video_path, image);
}


// Define a trivial destructor with 'default'
VideoProcessor::~VideoProcessor() = default;
