// ===============
// == R I L E Y ==
// ===============


#include "riley.h"


int main(int argc, char** argv) {

    std::cout << "Yessir.\n"
              << "Let's go!\n";

    if (argc == 5) {
        char *data_cfg = argv[2];
        char *cfg = argv[3];
        char *weights = argv[4];

        ObjectDetector detector(data_cfg, cfg, weights, 0);

        if (strcmp(argv[1], "yolo") == 0) {
            detector.detect();
            return 0;
        }
        if (strcmp(argv[1], "track") == 0) {

            detector.detect(true);
            return 0;
        }
    }

    if (argc == 6) {
        char *data_cfg = argv[2];
        char *cfg = argv[3];
        char *weights = argv[4];
        char *video = argv[5];

        ObjectDetector detector(data_cfg, cfg, weights, video);

        if (strcmp(argv[1], "yolo") == 0) {
            detector.detect();
            return 0;
        }
        if (strcmp(argv[1], "track") == 0) {
            detector.detect(true);
            return 0;
        }
    }

    std::cout << "[Usage]\n"
              << "[0] ./detector yolo <data_cfg> <cfg> <weights>\n"
              << "[1] ./detector yolo <data_cfg> <cfg> <weights> <video>.\n";

    return 1;
}
