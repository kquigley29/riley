// =====================
// ImageProcessor Source
// Process image data
// =====================


#include "../include/ImageProcessor.h"


ImageProcessor::ImageProcessor(const std::string &file_path) {

    // Find and assign the values of class label and number
    // Set the path
    int uscore = file_path.find("_");
    int dot = file_path.find(".");
    int len = dot - uscore;

    path = file_path;
    class_label = file_path.substr(uscore+1, len);
    class_number = std::stoi(file_path.substr(0,uscore-1));
}


std::string ImageProcessor::get_class_label() {return class_label;}
int ImageProcessor::get_class_number() {return class_number;}


void ImageProcessor::resize(const int &width, const int &height) {

    cv::Mat image;
    image = cv::imread(path);
    cv::Size image_size(width, height);
    cv::resize(image, image, image_size);
    cv::imwrite(path, image);
}


ImageProcessor::~ImageProcessor() = default;
