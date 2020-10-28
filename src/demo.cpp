// ===============
// == R I L E Y ==
// ===============


#include "riley.h"


void help() {
    std::cout << "[Usage]\n"
              << "[1] ./demo yolo\n"
              << "[2] ./demo track\n";
}


int main(int argc, char** argv) {
    bool yolo = strcmp(argv[1], "yolo") == 0;
    bool track = strcmp(argv[1], "track") == 0;

    if (!(yolo && track && argc == 2)){
        char data_cfg_default[] = "cfg/coco.data";
        char cfg_default[] = "cfg/riley.cfg";
        char weights_default[] = "weights/yolov3.weights";

        cv::VideoCapture cap(0);
        ObjectDetector detector(data_cfg_default, cfg_default, weights_default);
        detector.set_tracking(track);
        detector.detect_from_cap(cap);

        return 0;
    }
    help();
    return 1;
}
