// =========================
// YoloObjectDetector Source
// Detect objects. Fast
// =========================


#include "../include/YoloObjectDetector.h"


YoloObjectDetector::YoloObjectDetector(char *cfgfile, char *weightfile, char* video_source) {

    std::cout << "guys, my beehive died :(\n";
    std::cout << "            ~ pjreddie\n";

    net = load_network(cfgfile, weightfile, 0);
}


detection *YoloObjectDetector::detect(char *video_src) {

    image **alphabet = load_alphabet();
    ;

}


YoloObjectDetector::~YoloObjectDetector() = default;
