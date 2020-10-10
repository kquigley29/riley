// =================
// RileyUtils Header
// =================


#ifndef RILEY_UTILS_H
#define RILEY_UTILS_H


#include <darknet.h>
#include <opencv2/opencv.hpp>


cv::Mat image_to_mat(const image &img);

image mat_to_image(const cv::Mat &mat);

char *str_to_char_array(const std::string &str);

int size_network(network *net);

cv::VideoCapture *open_video_stream_from_camera(const int &index);

cv::VideoCapture *open_video_stream_from_link(const char *link);

image retrieve_image_from_stream(cv::VideoCapture *vc);


#endif //RILEY_UTILS_H
