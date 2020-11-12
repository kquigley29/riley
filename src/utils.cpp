#include <darknet.h>
#include "riley/utils.h"


namespace riley
{

    /**
     * Convert Darknet image to cv::Mat
     */
    cv::Mat image_to_mat(const image &img)
    {
        int channels = img.c;
        int width = img.w;
        int height = img.h;
        cv::Mat mat = cv::Mat(height, width, CV_8UC(channels));
        int step = mat.step;

        for (int y = 0; y < img.h; ++y)
        {
            for (int x = 0; x < img.w; ++x)
            {
                for (int c = 0; c < img.c; ++c)
                {
                    float val = img.data[c*img.h*img.w + y*img.w + x];
                    mat.data[y*step + x*img.c + c] = (unsigned char)(val * 255);
                }
            }
        }
        return mat;
    }


    /**
     * Convert cv::Mat to Darknet image
     */
    image mat_to_image(const cv::Mat &mat)
    {
        int w = mat.cols;
        int h = mat.rows;
        int c = mat.channels();
        image im = make_image(w, h, c);
        auto *data = (unsigned char *)mat.data;
        int step = mat.step;
        for (int y = 0; y < h; ++y)
        {
            for (int k = 0; k < c; ++k)
            {
                for (int x = 0; x < w; ++x)
                {
                    im.data[k*w*h + y*w + x] = data[y*step + x*c + k] / 255.0f;
                }
            }
        }
        return im;
    }


    /**
     * Convert std::string to a char[]
     */
    char *str_to_char_array(const std::string &str)
    {
        int n = str.length();
        char *char_array = (char *) malloc((n + 1) * sizeof(char));
        std::strcpy(char_array, str.c_str());
        return char_array;
    }


    /**
     * Append Darknet directory path to filename
     */
    char *append_darknet(char *filename)
    {
        return str_to_char_array(std::string(DARKNET) + std::string(filename));
    }

}