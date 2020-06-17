// ===============
// == R I L E Y ==
// ===============


#define TRAINING_LOOP_LIMIT 5


// Includes
#include <iostream>
#include <string>

#include "manipulate/include/VideoProcessor.h"
#include "network/include/RileyNet.h"
//#include "darknet_riley/include/YoloObjectDetector.h"


int main(int argc, char** argv) {
    std::cout << "Yessir."       << "\n\n"
              << "L        EEEEEEE   TTTTTTT   ,,,    sssss          GGGGG     OOOOO    !!!!!" << "\n"
              << "L        E            T       ,    s     s        G     G   O     O   !!!!!" << "\n"
              << "L        EEEEE        T            s              G         O     O    !!! " << "\n"
              << "L        EEEEE        T            sssssss        G    GG   O     O    !!! " << "\n"
              << "L        E            T                  s        G     G   O     O     !  " << "\n"
              << "LLLLLLL  EEEEEEE      T             sssss          GGGGG     OOOOO      0  " << "\n\n";

    // Sort out the arguments
    std::string invalid_args {"Invalid arguments!"};
    if (argc < 2) {std::cout << invalid_args << "\n"; return 1;}
    std::string method = argv[1];

    if (method == "rgb") {
        if (argc == 3) {

            std::cout << "RGB Stream...\n";
            // Stream the rgb of the video pixels
            std::string video_name = argv[2];
            VideoProcessor processor(video_name);
            processor.rgb_stream();
        }
    }
    else if (method == "fetch") {
        if (argc == 3) {

            std::cout << "Fetching...\n";
            // Fetch the training data of the video
            std::string video_name = argv[2];
            VideoProcessor processor(video_name);
            OpenNN::Matrix<double> data;
            data = processor.fetch_data_matrix();
            data.print();
        }
    }
    else if (method == "train") {
        if (argc == 5) {

            std::cout << "Training...\n";
            std::string net_xml = argv[2];
            std::string train_xml = argv[3];
            std::string list_file_name = argv[4];
            std::ifstream list_file(list_file_name);

            for (int i = 0; i < TRAINING_LOOP_LIMIT; i++) {
                RileyNet nn_1(net_xml, train_xml); /* Because Ryan likes numbers */
                nn_1.print_architecture();

                std::vector<std::string> data_mp4_names;
                std::string list_file_line;
                // Read the next line from File until it reaches the end.
                while (std::getline(list_file, list_file_line)) {

                    // Line contains string of length > 0 then save it in vector
                    if (!list_file_line.empty())
                        data_mp4_names.push_back(list_file_line);
                }

                for (const std::string &file_name : data_mp4_names) {

                    // Train on the data from VideoProcessor::fetch_data_matrix
                    VideoProcessor processor(file_name);
                    OpenNN::Matrix<double> data = processor.fetch_data_matrix();
                    nn_1.setup_and_train(data);
                    std::cout << "Training complete on matrix!\n";
                }
            }
        }
    }
    else if (method == "collect") {
        if (argc == 4) {

            std::cout << "Collecting...\n";
            std::string video_name = argv[2];
            std::string image_name_piece = argv[3];
            VideoProcessor processor(video_name);
            processor.save_frames(image_name_piece);
        }
    }
    // If nothing matches the arguments tell us!
    else {
        std::cout << invalid_args << "\n";
        return 1;
    }

    return 0;
}
