// ---------------
// == R I L E Y ==
// ---------------

// System includes
#include <iostream>
#include <cmath>

// Header includes
/* Currently none */

// Namespaces
/* Currently none */

int main(int argc, char** argv)
{
    std::cout << "Yessir."       << std::endl
              << "Let's GOOOOO!" << std::endl;

    for (int i=0; i<argc; i++)
    {
        std::cout << argv[i] << std::endl;
    }

    return 0;
}