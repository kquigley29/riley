// =====================
// ImageProcessor Source
// Process image data
// =====================


#include "../include/ImageProcessor.h"


ImageProcessor::ImageProcessor(const std::string &file_path) {

    // Find and assign the values of class label and number
    // Set the path
    path = file_path;
}


std::string ImageProcessor::get_path() {return path;}
void ImageProcessor::set_path(const std::string &new_path) {path = new_path;}


void ImageProcessor::resize(const int &width, const int &height, std::string new_path, const int &count, const std::string &image_class) {
    cv::Mat image;
    image = cv::imread(path);
    cv::Size image_size(width, height);
    cv::resize(image, image, image_size);
    new_path = new_path + "/" + std::to_string(count) + "_" + image_class + ".jpg";
    cv::imwrite(new_path, image);
}


ImageProcessor::~ImageProcessor() = default;
