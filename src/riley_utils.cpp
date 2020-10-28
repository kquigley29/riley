#include <darknet.h>
#include "riley_utils.h"


cv::Mat image_to_mat(const image &img) {
    /*
     Converts a darknet image to an OpenCV Mat.
     */
    assert(img.c == 3 || img.c == 1);
    int x, y, c;
    image copy = copy_image(img);
    constrain_image(copy);
    if (img.c == 3) rgbgr_image(copy);
    cv::Mat m(img.h, img.w, CV_MAKETYPE(CV_8U, img.c));
    for (y = 0; y < img.h; ++y) {
        for (x = 0; x < img.w; ++x) {
            for (c = 0; c < img.c; ++c) {
                float val = copy.data[c * img.h * img.w + y * img.w + x];
                m.data[y * img.w * img.c + x * img.c + c] = (unsigned char) (val * 255);
            }
        }
    }
    free_image(copy);
    return m;
}


image mat_to_image(const cv::Mat &m) {
    /*
     Converts an OpenCV Mat to a darknet image.
     */
    int h = m.rows;
    int w = m.cols;
    int c = m.channels();
    image im = make_image(w, h, c);
    auto *data = (unsigned char *) m.data;
    int step = m.step;
    int i, j, k;

    for (i = 0; i < h; ++i) {
        for (k = 0; k < c; ++k) {
            for (j = 0; j < w; ++j) {
                im.data[k * w * h + i * w + j] = data[i * step + j * c + k] / 255.;
            }
        }
    }
    rgbgr_image(im);
    return im;
}


char *str_to_char_array(const std::string &str) {
    /*
    Converts a standard string to a character array.
    */
    int n = str.length();
    char *char_array = (char *) malloc((n + 1) * sizeof(char));
    std::strcpy(char_array, str.c_str());
    return char_array;
}


int size_network(network *net) {
    /*
     Returns the size of the network passed as the argument.
     */
    int count = 0;
    for(int i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        if(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            count += l.outputs;
        }
    }
    return count;
}
