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
    std::string get_path();
    void set_path(const std::string&);
    void resize(const int&, const int&, std::string, const int&, const std::string&);

private:
    std::string path;
};


#endif //RILEY_IMAGEPROCESSOR_H
