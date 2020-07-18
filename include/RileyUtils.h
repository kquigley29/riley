// ==========================
// darknet_riley utils header
// ==========================


#include <darknet.h>
#include <opencv2/opencv.hpp>


#ifndef RILEY_UTILS_H
#define RILEY_UTILS_H

cv::Mat image_to_mat(const image &img);
image mat_to_image(const cv::Mat &mat);
char *str_to_char_array(const std::string &str);


#endif //RILEY_UTILS_H
