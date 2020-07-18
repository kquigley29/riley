// =================
// RileyUtils Header
// =================


#include <darknet.h>
#include <demo.h>
#include <opencv2/opencv.hpp>


#ifndef RILEY_UTILS_H
#define RILEY_UTILS_H

cv::Mat image_to_mat(const image &img);
image mat_to_image(const cv::Mat &mat);

char *str_to_char_array(const std::string &str);

int size_network(network *net);

void *open_video_stream_from_camera(const int &index);
void *open_video_stream_from_link(const char *link);
image get_image_from_stream(void *p);



#endif //RILEY_UTILS_H
