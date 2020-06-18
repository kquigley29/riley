// =====================
// MediaProcessor Source
// Process video data
// =====================


// Includes
#include "../include/MediaProcessor.h"


// MediaProcessor constructor
MediaProcessor::MediaProcessor(const cv::String& name) {

    std::cout << "<<LOAD IT UP>>\n";

    // Get the video file name
    path = name;

    std::cout << "MediaProcessor object created with video_path=" << path << "\n";
}


// Process video into 300x300 sized RGB frames
void MediaProcessor::rgb_stream() {
    // Capture the video
    cv::VideoCapture capture(path);

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


OpenNN::Matrix<int> MediaProcessor::fetch_data_matrix() {

    get_target();

    const std::vector<int> wheel = {0, 32, 15, 19, 4, 21, 2, 25, 17, 34, 6, 27, 13, 36, 11, 30, 8, 23, 10, 5, 24, 16, 33, 1, 20, 14, 31, 9, 22, 18, 29, 7, 28, 12, 35, 3, 26};

    int index = permute_wheel(target);
    int upper = (index + WHEEL_BUFFER) % 37;
    int lower = (index - WHEEL_BUFFER) % 37;

    // Declare the target vector and populate it
    OpenNN::Vector<int> target_vector(37);
    if (upper > lower) {
        for (int i = 0; i < 37; i++) {
            if (i >= lower && i <= upper) target_vector[wheel[i]] = 1;
            else target_vector[wheel[i]] = 0;
        }
    }
    else if (upper < lower) {
        for (int i = 0; i < 37; i++) {
            if (i >= lower || i <= upper) target_vector[wheel[i]] = 1;
            else target_vector[wheel[i]] = 0;
        }
    }

    // Capture the video
    cv::VideoCapture capture(path);

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
    OpenNN::Matrix<int> data_matrix(NUM_FRAMES_FOR_DATA * 3,
                                     FRAME_HEIGHT * FRAME_WIDTH + target_vector.size());
    cv::Mat frame;
    int red_index = 0;
    int green_index = 1;
    int blue_index = 2;
    cv::Size frame_size(FRAME_WIDTH, FRAME_HEIGHT);
    for (int i = 0; i < NUM_FRAMES_FOR_DATA; i++) {
        // Assign values to the image matrix
        // Resize and convert to RGB
        capture.read(frame);
        cv::resize(frame, frame, frame_size, 0, 0, cv::INTER_NEAREST);
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

        cv::imshow("Frame", frame);
        cv::waitKey(1000);


        // Declare data_matrix to store RGB with the correct size
        // Declare vectors to store RGB values
        // Declare pixel_count to count the pixels
        OpenNN::Vector<int> red_vec(FRAME_WIDTH * FRAME_HEIGHT),
                               green_vec(FRAME_WIDTH * FRAME_HEIGHT),
                               blue_vec(FRAME_WIDTH * FRAME_HEIGHT);
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

                if (red > 255) red = 255;
                if (green > 255) green = 255;
                if (blue > 255) blue = 255;
                if (red < 0) red = 0;
                if (green < 0) green = 0;
                if (blue < 0) blue = 0;


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

        std::cout << "blue_vec end: " << blue_vec[blue_vec.size()-1] << "\n";
    }

//    for (int m : data_matrix) {
//        if (m > 255) m = 255;
//        else if (m < 0) m = 0;
//    }
    return data_matrix;
}


void MediaProcessor::save_frames(const std::string& image_name) {

    cv::VideoCapture capture(path);
    int count = 0;
    cv::Mat frame;
    while (capture.read(frame)) {

        std::string file = "/home/keane/Projects/riley/data/test_images/" + to_string(count) + image_name;
        cv::imwrite(file, frame);
        count++;
    }

    std::cout << "Finished!\n";
}


void MediaProcessor::resize(const int &width, const int &height) {

    cv::Mat image;
    image = cv::imread(path);
    cv::Size image_size(width, height);
    cv::resize(image, image, image_size);

    cv::imwrite(path, image);
}


void MediaProcessor::get_target() {

    // Get the target value from the video name
    std::string target_string = path.substr(18, 2);
    target = std::stoi(target_string);

    std::cout << "Target: " << target << "\n";
}


// Define a trivial destructor with 'default'
MediaProcessor::~MediaProcessor() = default;


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

