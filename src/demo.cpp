#include "riley/riley.h"


using namespace riley;


void print_help() {
    std::cout << "[Usage]\n"
              << "./demo yolo\n";
}


int main(int argc, char** argv)
{
    if (argc == 2 && strcmp(argv[1], "yolo") == 0)
    {
        ObjectDetector detector;
        detector.detect_stream();
        return 0;
    }
    print_help();
    return 1;
}
