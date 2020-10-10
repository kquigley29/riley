// ===============
// == R I L E Y ==
// ===============


#include "riley.h"


void help() {
    std::cout << "[Usage]\n"
              << "[1] ./detector yolo <data_cfg> <cfg> <weights>\n"
              << "[2] ./detector yolo <data_cfg> <cfg> <weights> <video>.\n"
              << "[3] ./detector track <data_cfg> <cfg> <weights>\n"
              << "[4] ./detector track <data_cfg> <cfg> <weights> <video>\n";
}


int main(int argc, char** argv) {
    bool yolo = strcmp(argv[1], "yolo") == 0;
    bool track = strcmp(argv[1], "track") == 0;

    if (yolo || track){
        ObjectDetector detector;
        char data_cfg_default[] = "cfg/coco.data";
        char cfg_default[] = "cfg/riley.cfg";
        char weights_default[] = "weights/yolov3.weights";

        switch (argc) {
            case 2:
                detector = ObjectDetector(data_cfg_default, cfg_default, weights_default);
                break;

            case 5:
                detector = ObjectDetector(argv[2], argv[3], argv[4]);
                break;

            case 6:
                detector = ObjectDetector(argv[2], argv[3], argv[4], (argv[5]));
                break;

            default:
                help();
                return 1;
        }
        detector.set_tracking(track);
        detector.display();
        return 0;
    }
    help();
    return 1;
}
