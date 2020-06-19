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
              << "./manipulate fetch <video file>\n";
}


int main(int argc, char** argv) {

    std::cout << "WHIP IT UP\n";

    if (argc <= 2) help();
    std::string method = argv[1];

    if (method == "fetch") {
       std::string video_file = argv[2];
       VideoProcessor processor(video_file);
       OpenNN::Matrix<double> data = processor.get_random_data_matrix();
       data.print();

       return 0;
    }

    else help();
}