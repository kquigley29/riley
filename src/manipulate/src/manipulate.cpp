//
// Created by keane on 17/06/2020.
//

#include <string>
#include <iostream>
#include "../include/MediaProcessor.h"


void invalid() {std::cout << "Invalid arguments!\n";}


int main(int argc, char** argv) {

    if (argc < 2) {invalid(); return 1;}

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

                return 0;
            }

            invalid();
        }

        invalid();
    }

    else if (method == "fetch") {
        if (argc == 3) {

            std::cout << "Fetching...\n";
            // Fetch the training data of the video
            std::string video_name = argv[2];
            MediaProcessor processor(video_name);
            OpenNN::Matrix<double> data;
            data = processor.fetch_data_matrix();
            data.print();

            return 0;
        }

        invalid();
    }

    else if (method == "collect") {
        if (argc == 4) {

            std::cout << "Collecting...\n";
            std::string video_name = argv[2];
            std::string image_name_piece = argv[3];
            MediaProcessor processor(video_name);
            processor.save_frames(image_name_piece);

            return 0;
        }

        invalid();
    }

    else if (method == "rgb") {
        if (argc == 3) {

            std::cout << "RGB Stream...\n";
            // Stream the rgb of the video pixels
            std::string video_name = argv[2];
            MediaProcessor processor(video_name);
            processor.rgb_stream();

            return 0;
        }

        invalid();
    }

    invalid();
}