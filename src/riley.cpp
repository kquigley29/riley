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
#include <cmath>

#include "data_process/include/VideoProcessor.h"
#include "darknet_riley/include/YoloObjectDetector.h"


int main(int argc, char** argv)
{
    std::cout << "Yessir."       << "\n"
              << "Let's GOOOOO!" << "\n";

    std::string invalid_args {"Invalid arguments!"};

    if (argc == 1)
    {
        std::cout << invalid_args << "\n";
    }

    else if (argc == 2)
    {
        std::cout << invalid_args << "\n";
    }

    else if (argc == 3)
    {
        std::string arg1 {argv[1]};
        std::string arg2 (argv[2]);

        // If arg1 (method) is 'process', use VideoProcess class
        // found in riley/src/data_process/VideoProcess.cpp
        if (arg1 == "process")
        {
            VideoProcessor processor(arg2);
            processor.process();
        }

        else
        {
            std::cout << invalid_args << "\n";
        }
    }

    else if (argc == 5)
    {
        std::string arg1 {argv[1]};
        std::string arg2 {argv[2]};
        std::string arg3 {argv[3]};
        std::string arg4 {argv[4]};

        if (arg1 == "detect")
        {
            YoloObjectDetector detector(arg2, arg3, arg4);
            detector.detect();
        }

        else
        {
            std::cout << invalid_args << "\n";
        }
    }

    else
    {
        std::cout << invalid_args << "\n";
    }

    return 0;
}
