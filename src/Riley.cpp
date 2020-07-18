// ===============
// == R I L E Y ==
// ===============


// Includes
#include <Riley.h>


void help() {

    std::cout << "[Usage]\n"
              << "[0] ./riley yolo <data> <cfg> <weights>"
              << "[1] ./riley yolo <data> <cfg> <weights> <video>.";
}


int main(int argc, char** argv) {

    std::cout << "Yessir."       << "\n\n"
              << "L        EEEEEEE   TTTTTTT   ,,,    sssss          GGGGG     OOOOO    !!!!!" << "\n"
              << "L        E            T       ,    s     s        G     G   O     O   !!!!!" << "\n"
              << "L        EEEEE        T            s              G         O     O    !!! " << "\n"
              << "L        EEEEE        T            sssssss        G    GG   O     O    !!! " << "\n"
              << "L        E            T                  s        G     G   O     O     !  " << "\n"
              << "LLLLLLL  EEEEEEE      T             sssss          GGGGG     OOOOO      0  " << "\n\n";

    auto yolo = "yolo";
    if (strcmp(argv[1], yolo) == 0 && argc == 5) {
        char *data_cfg = argv[2];
        char *cfg = argv[3];
        char *weights = argv[4];

        YoloObjectDetector detector(data_cfg, cfg, weights);
        detector.detect(nullptr, 0);
        return 0;
    }

    if (strcmp(argv[1], yolo) == 0 && argc == 6) {
        char *data_cfg = argv[2];
        char *cfg = argv[3];
        char *weights = argv[4];
        char *video = argv[5];

        YoloObjectDetector detector(data_cfg, cfg, weights);
        detector.detect(video, -1);
        return 0;
    }

    help();
    return 1;
}
