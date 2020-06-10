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

    std::string invalid_args {"Invalid arguments!"};

    if (argc == 1)
    {
        std::cout << invalid_args << std::endl;
    }

    else if (argc == 2)
    {
        std::cout << invalid_args << std::endl;
    }

    else if (argc == 3)
    {
        std::string arg1 {argv[1]};
        std::string arg2 (argv[2]);

        // If arg1 (method) is process, use VideoProcess class
        // found in riley/src/data_process/VideoProcess.cpp
        if (arg1 == "process")
        {
            VideoProcessor process(arg2);
        }

        else
        {
            std::cout << invalid_args << std::endl;
        }
    }

    else
    {
        std::cout << invalid_args << std::endl;
    }

    return 0;
}
