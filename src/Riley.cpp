// ===============
// == R I L E Y ==
// ===============


// Includes
#include <Riley.h>


void help() {

    std::cout << "[Usage]\n"
              << "[0] ./riley yolo <data_cfg> <cfg> <weights>"
              << "[1] ./riley yolo <data_cfg> <cfg> <weights> <video>.";
}


int main(int argc, char** argv) {

    std::cout << "Yessir.\n"
              << "Let's go!\n";

    auto yolo = "yolo";
    if (strcmp(argv[1], yolo) == 0 && argc == 5) {
        char *data_cfg = argv[2];
        char *cfg = argv[3];
        char *weights = argv[4];

        ObjectDetector detector(data_cfg, cfg, weights);
        detector.detect(nullptr, 0);
        return 0;
    }

    if (strcmp(argv[1], yolo) == 0 && argc == 6) {
        char *data_cfg = argv[2];
        char *cfg = argv[3];
        char *weights = argv[4];
        char *video = argv[5];

        ObjectDetector detector(data_cfg, cfg, weights);
        detector.detect(video, -1);
        return 0;
    }

    help();
    return 1;
}
