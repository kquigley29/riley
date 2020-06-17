//
// Created by keane on 17/06/2020.
//

#include <string>
#include <iostream>
#include "../include/VideoProcessor.h"


int main(int argc, char** argv) {

    std::string method = argv[1];

    if (method == "resize") {
        if (argc == 5) {

            std::string image_name = argv[2];
            int width = std::stoi(argv[3]);
            int height = std::stoi(argv[4]);
            cv::Mat image = cv::imread(image_name);
            cv::Size image_size(width, height);
            cv::resize(image, image, image_size);

            cv::imwrite(image_name, image);

            return 0;
        }
        else if (argc == 6) {
            std::string all = argv[2];
            if (all == "-r") {
                std::string list_file_name = argv[3];
                int width = std::stoi(argv[4]);
                int height = std::stoi(argv[5]);

                std::string line;
                std::ifstream list_file(list_file_name);

                while (std::getline(list_file, line)) {

                    cv::Mat image = cv::imread(line);
                    cv::Size image_size(width, height);
                    cv::resize(image, image, image_size);

                    cv::imwrite(line, image);
                }
            }
        }
    }
    else {

        std::cout << "Invalid arguments!\n";
        return 1;
    }
}