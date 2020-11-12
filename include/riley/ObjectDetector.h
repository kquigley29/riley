#ifndef RILEY_OBJECTDETECTOR_H
#define RILEY_OBJECTDETECTOR_H


#include <opencv2/opencv.hpp>
#include <darknet.h>

namespace riley
{

    class ObjectDetector {
    public:
        explicit ObjectDetector();

        explicit ObjectDetector(char *cfg, char *names, char *weights, const bool &from_darknet = false);

        void detect(const cv::Mat &input_image, cv::Mat &output_image);

        void detect_stream();

        void set_thresh(const float &new_thresh);

    private:
        char **detect_labels;
        image **detect_alphabet;
        network *net;
        float detect_thresh;
        float detect_hier;
        float detect_nms;
        layer l;
    };

}


#endif //RILEY_OBJECTDETECTOR_H

