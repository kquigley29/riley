// =========================
// == M A N I P U L A T E ==
// =========================


//Includes
#include <iostream>
#include <string>

#include "../include/VideoProcessor.h"
#include "../include/ImageProcessor.h"


void help() {

    std::cout << "[Usage]\n"
              << "[0] ./manipulate fetch <video file>\n"
              << "[1] ./manipulate collect <video file> <image directory>\n"
              << "[2] ./manipulate resize <image file> <width> <height>\n";
}


int main(int argc, char** argv) {

    std::cout << "WHIP IT UP\n";

    if (argc <= 2) help();
    std::string method = argv[1];

    if (method == "fetch") {
       std::string video_file = argv[2];
       VideoProcessor vprocessor(video_file);
       OpenNN::Matrix<double> data = vprocessor.get_random_data_matrix();
       data.print();

       return 0;
    }

    if (method == "collect") {
        std::string video_file = argv[2];
        std::string image_dir = argv[3];
        VideoProcessor vprocessor(video_file);
        vprocessor.collect_frames(image_dir);

        return 0;
    }

    if (method == "resize") {
        std::string image_file = argv[2];
        int width = std::stoi(argv[3]);
        int height = std::stoi(argv[4]);
        ImageProcessor iprocessor(image_file);
        iprocessor.resize(width, height);

        return 0;
    }

    help();
    return 1;
}