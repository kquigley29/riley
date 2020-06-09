// ===============
// == R I L E Y ==
// ===============


// =======================================================================================================
// Compile command
// g++ src/riley.cpp src/data_process/VideoProcessor.cpp -o riley.exe `pkg-config opencv4 --cflags --libs`
//========================================================================================================


// System includes
#include <iostream>
#include <string>
#include <cmath>

// Header includes
#include "data_process/VideoProcessor.h"

// Namespaces
/* Currently none */

int main(int argc, char** argv)
{
    std::cout << "Yessir."       << std::endl
              << "Let's GOOOOO!" << std::endl;

    std::cout << "argc = " << argc << std::endl;

    if (argc == 1)
    {
        std::cout << ":(" << std::endl;
    }

    if (argc == 2)
    {
        std::cout << ":(" << std::endl;
    }

    if (argc == 3)
    {
        std::string method = argv[1];
        std::string argument = argv[2];


        // If the method is process, use process function found in
        // riley/src/data_process/process.h
        if (method == "process") {
            VideoProcessor process(argument);
        }
    }

    return 0;
}
