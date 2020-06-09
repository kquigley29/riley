// ---------------
// == R I L E Y ==
// ---------------

// System includes
#include <iostream>
#include <string>
#include <cmath>

// Header includes
#include "data_process/process.h"

// Namespaces
/* Currently none */

int main(int argc, char** argv)
{
    std::cout << "Yessir."       << std::endl
              << "Let's GOOOOO!" << std::endl;

    std::string method = argv[1];
    std::string argument = argv[2];


    // If the method is process, use process function found in
    // riley/src/data_process/process.h
    if (method == "process")
    {
        Data::process(argument);
    }

    return 0;
}