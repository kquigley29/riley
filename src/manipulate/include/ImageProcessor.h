// =====================
// ImageProcessor Header
// Process image data
// =====================


// Includes
#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>


#ifndef RILEY_IMAGEPROCESSOR_H
#define RILEY_IMAGEPROCESSOR_H


class ImageProcessor {
public:
    explicit ImageProcessor(const std::string&);
    virtual ~ImageProcessor();

    // Member functions
    std::string get_class_label();
    int get_class_number();
    void resize(const int&, const int&);

private:
    std::string path;
    std::string class_label;
    int class_number;

};


#endif //RILEY_IMAGEPROCESSOR_H
