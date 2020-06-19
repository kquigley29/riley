// ===============
// == R I L E Y ==
// ===============


// Macro definitions
#define TRAINING_LOOP_LEN_ 5


// Includes
#include <iostream>
#include <string>

#include "manipulate/include/VideoProcessor.h"
#include "manipulate/include/ImageProcessor.h"
#include "network/include/RileyNet.h"
//#include "darknet_riley/include/YoloObjectDetector.h"


void help() {

    std::cout << "[Usage]\n"
              << "[0] ./train <video list file>";
}


int main(int argc, char** argv) {

    std::cout << "Yessir."       << "\n\n"
              << "L        EEEEEEE   TTTTTTT   ,,,    sssss          GGGGG     OOOOO    !!!!!" << "\n"
              << "L        E            T       ,    s     s        G     G   O     O   !!!!!" << "\n"
              << "L        EEEEE        T            s              G         O     O    !!! " << "\n"
              << "L        EEEEE        T            sssssss        G    GG   O     O    !!! " << "\n"
              << "L        E            T                  s        G     G   O     O     !  " << "\n"
              << "LLLLLLL  EEEEEEE      T             sssss          GGGGG     OOOOO      0  " << "\n\n";

    if (argc <= 1) {help(); return 1;}
    std::string method = argv[1];

    if (method == "train") {
        std::string list_file = argv[2];
        std::ifstream list_file_stream(list_file);
        RileyNet nn_1;
        nn_1.print_architecture();

        for (int i = 0; i != TRAINING_LOOP_LEN_; i++) {
            std::string line;
            while (std::getline(list_file_stream, line)) {
                if (!line.empty()) {
                    VideoProcessor vprocessor(line);
                    OpenNN::Matrix<double> data = vprocessor.get_random_data_matrix();
                    nn_1.educate(data);
                }
            }
        }

        return 0;
    }

    help();
    return 1;
}
