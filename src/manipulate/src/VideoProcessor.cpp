// =====================
// VideoProcessor Source
// Process video data
// =====================


// Includes
#include "../include/VideoProcessor.h"


VideoProcessor::VideoProcessor(const string & file_path) {

    std::cout << "<<LOAD IT UP>>\n";

    // Set path to the file path
    path = file_path;
    cap = new cv::VideoCapture(path);
    // TODO: initialise the target
}

int VideoProcessor::get_target() const {return target;}
std::string VideoProcessor::get_path() {return path;}
void VideoProcessor::set_path(const std::string &new_path) {path = new_path;}


OpenNN::Matrix<double_t> VideoProcessor::get_random_data_matrix() {

    // Initialise a matrix to store the data and populate the target columns
    OpenNN::Matrix<double_t> data_matrix(3*NUM_FRAMES_OF_DATA_,
                                       VIDEO_FRAME_WIDTH_*VIDEO_FRAME_HEIGHT_ + NUM_WHEEL_POCKETS_);

    // Populate the matrix with the target data
    int target_index = permute_wheel(target);
    int upper_limit = (target_index + TARGET_INCLUSION_BUFFER_) % NUM_WHEEL_POCKETS_;
    int lower_limit = (target_index - TARGET_INCLUSION_BUFFER_) % NUM_WHEEL_POCKETS_;

    for (int i = 0; i != data_matrix.get_rows_number(); i++) {
        for (int j = 0; j != NUM_WHEEL_POCKETS_; j++) {

            if (upper_limit >= lower_limit) {
                if (j <= upper_limit && j >= lower_limit) data_matrix(i,wheel[j]) = 1;
                else data_matrix(i, wheel[j]) = 0;
            }
            else if (upper_limit < lower_limit) {
                if (j <= upper_limit || j >= lower_limit) data_matrix(i, wheel[j]) = 1;
                else data_matrix(i, wheel[j]) = 0;
            }
        }
    }

    // Capture the video
    // Find a random frame to start at and skip all preceding frames
    cv::Mat frame;
    int num_frames = cap->get(cv::CAP_PROP_FRAME_COUNT);
    int rand_num = (rand() % num_frames) - RANDOM_NUMBER_BUFFER_;
    int frame_count = 0;
    while (frame_count != rand_num) {cap.read(frame); frame_count++;}

    // Set the frame size for the images
    // Collect the rgb data from the frames
    cv::Size frame_size(VIDEO_FRAME_WIDTH_, VIDEO_FRAME_HEIGHT_);
    for (int i = 0; i != NUM_FRAMES_OF_DATA_; i++) {

        // Resize the frame and adjust colour scheme
        cap->read(frame);
        cv::resize(frame, frame, frame_size);
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

        // Get the RGB values of all pixels
        int pixel_count = 0;
        for (int row = 0; row != frame.rows; row++) {

            const cv::Vec3b *Mi = frame.ptr<cv::Vec3b>(row);

            for (int col = 0; col != frame.cols; col++) {

                cv::Vec3b pixel = Mi[col];
                double red = pixel[0];
                double green = pixel[1];
                double blue = pixel[2];

                // Populate the matrix
                data_matrix(3*i, pixel_count + NUM_WHEEL_POCKETS_) = red;
                data_matrix(3*i + 1, pixel_count + NUM_WHEEL_POCKETS_) = green;
                data_matrix(3*i + 2, pixel_count + NUM_WHEEL_POCKETS_) = blue;

                pixel_count++;
            }
        }
    }

    return data_matrix;
}


void VideoProcessor::collect_frames(const std::string &image_dir) {

    // Capture the image
    cv::VideoCapture cap(path);
    cv::Mat frame;
    int frame_count = 0;

    // Write each frame to the give directory
    while (cap.read(frame)) {
        std::string image_path = image_dir + "/" + path.substr(12,8) + "_" + to_string(frame_count) + ".jpg";
        cv::imwrite(image_path, frame);
        frame_count++;
    }
}


VideoProcessor::~VideoProcessor() = default;


// For finding the neighbouring pockets
// Used in VideoProcessor::get_random_data_matrix()
int permute_wheel(int n) {

    if (n == 0) return 0;
    else if (n == 32) return 1;
    else if (n == 15) return 2;
    else if (n == 19) return 3;
    else if (n == 4) return 4;
    else if (n == 21) return 5;
    else if (n == 2) return 6;
    else if (n == 25) return 7;
    else if (n == 17) return 8;
    else if (n == 34) return 9;
    else if (n == 6) return 10;
    else if (n == 27) return 11;
    else if (n == 13) return 12;
    else if (n == 36) return 13;
    else if (n == 11) return 14;
    else if (n == 30) return 15;
    else if (n == 8) return 16;
    else if (n == 23) return 17;
    else if (n == 10) return 18;
    else if (n == 5) return 19;
    else if (n == 24) return 20;
    else if (n == 16) return 21;
    else if (n == 33) return 22;
    else if (n == 1) return 23;
    else if (n == 20) return 24;
    else if (n == 14) return 25;
    else if (n == 31) return 26;
    else if (n == 9) return 27;
    else if (n == 22) return 28;
    else if (n == 18) return 29;
    else if (n == 29) return 30;
    else if (n == 7) return 31;
    else if (n == 28) return 32;
    else if (n == 12) return 33;
    else if (n == 35) return 34;
    else if (n == 3) return 35;
    else if (n == 26) return 36;

    return -1;
}
