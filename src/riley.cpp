// ===============
// == R I L E Y ==
// ===============


// =======================================================================================================
// Compile command
// g++ src/riley.cpp src/data_process/VideoProcessor.cpp -o riley.exe `pkg-config opencv4 --cflags --libs`
// =======================================================================================================


// Includes
#include <iostream>
#include <string>

#include "data_process/include/VideoProcessor.h"
//#include "darknet_riley/include/YoloObjectDetector.h"


int main(int argc, char** argv)
{
    std::cout << "Yessir."       << "\n"
              << "Let's  GGGGG     OOOOO    !!!!!" << "\n"
              << "      G     G   O     O   !!!!!" << "\n"
              << "      G         O     O    !!! " << "\n"
              << "      G    GG   O     O    !!! " << "\n"
              << "      G     G   O     O     !  " << "\n"
              << "       GGGGG     OOOOO      0  " << "\n";

    std::string invalid_args {"Invalid arguments!"};
    std::string method;

    if (argc >= 2) method = argv[1];
    else {std::cout << invalid_args; return 1;}

    // If the method is process and there is a video name
    if (argc == 3 && method == "process")
    {
        // Process the video according to VideoProcessor::process()
        std::string video_name = argv[2];
        VideoProcessor processor(video_name);
        processor.process();
    }
    else if (argc == 3 && method == "training_data")
    {
        std::string video_name = argv[2];
        VideoProcessor processor(video_name);
        OpenNN::Matrix<double> data;
        data = processor.get_training_data();
        data.print_preview();
    }
    // If nothing matches the arguments tell us!
    else
    {
        std::cout << invalid_args << "\n";
        return 1;
    }

    return 0;
}
