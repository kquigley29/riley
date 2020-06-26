// =========================
// == M A N I P U L A T E ==
// =========================


//Includes
#include <iostream>
#include <string>

#include "../include/manipulate.h"


void help() {

    std::cout << "[Usage]\n"
              << "[0] ./manipulate fetch <video file>\n    // Get a data matrix\n"
              << "[1] ./manipulate collect <video file> <image directory>\n    // Get frames from a video\n"
              << "[2] ./manipulate resize <image file> <width> <height> <output dir> <start count> <image class>\n    // Resize one image\n"
              << "[3] ./manipulate resize many <list file> <width> <height> <output dir> <start count> <image class>\n    // Resize multiple images from a list file\n";
}


int main(int argc, char** argv) {

    std::cout << "WHIP IT UP\n";

    if (argc <= 2) {help(); return 1;}
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
        std::string arg2 = argv[2];
        if (arg2 == "many") {
            std::string list_file = argv[3];
            int width = std::stoi(std::string(argv[4]));
            int height = std::stoi(std::string(argv[5]));
            std::string dir = argv[6];
            int count = std::stoi(std::string(argv[7]));
            std::string image_class = argv[8];
            std::ifstream list_file_stream(list_file);
            for (std::string line; std::getline(list_file_stream, line); ) {
                if (!line.empty()) {
                    ImageProcessor iprocessor(line);
                    iprocessor.resize(width, height, dir, count, image_class);
                    std::cout << "Count: " << count << "\n";
                    count++;
                }
            }
            return 0;
        }

        std::string image_file = argv[2];
        int width = std::stoi(argv[3]);
        int height = std::stoi(argv[4]);
        std::string dir = argv[5];
        int count = std::stoi(argv[6]);
        std::string image_class = argv[7];
        ImageProcessor iprocessor(image_file);
        iprocessor.resize(width, height, dir, count, image_class);

        return 0;
    }

    help();
    return 1;
}