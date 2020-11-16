#include "riley/ObjectDetector.h"
#include "riley/utils.h"
#include <data.h>
#include <network.h>


namespace riley
{

    ObjectDetector::ObjectDetector()
    : detect_thresh(0.5)
    , detect_hier(0.5)
    , detect_nms(0.45)
    {
        char *cfg = append_darknet("cfg/yolov3-keane.cfg");
        char *weights = append_darknet("yolov3.weights");
        char *labels = append_darknet("data/coco.names");
        this->net = load_network(cfg, weights, 0);
        this->detect_alphabet = load_alphabet();
        this->detect_labels = get_labels(labels);
        set_batch_network(this->net, 1);
        this->l = this->net->layers[this->net->n - 1];
    }


    ObjectDetector::ObjectDetector(char *cfg, char *names, char *weights, const bool &from_darknet)
    : detect_thresh(0.5)
    , detect_hier(0.5)
    , detect_nms(0.45)
    {
        if (from_darknet)
        {
            this->net = load_network(append_darknet(cfg), append_darknet(weights), 0);
            this->detect_labels = get_labels(append_darknet(names));
        }
        else
        {
            this->net = load_network(cfg, weights, 0);
            this->detect_labels = get_labels(names);
        }
        this->detect_alphabet = load_alphabet();
        set_batch_network(this->net, 1);
        this->l = this->net->layers[this->net->n - 1];
    }


    std::pair<detection*, int> ObjectDetector::detect(const cv::Mat &input_image, cv::Mat &output_image)
    {
        image img = mat_to_image(input_image);
        image sized_img = letterbox_image(img, this->net->w, this->net->h);

        float *X = sized_img.data;
        network_predict(*this->net, X);
        int nboxes = 0;
        detection *dets = get_network_boxes(this->net, img.w, img.h, this->detect_thresh, this->detect_hier, nullptr, 1, &nboxes, 0);
        if (this->detect_nms != 0) do_nms_sort(dets, nboxes, this->l.classes, this->detect_nms);
        draw_detections_v3(img, dets, nboxes, this->detect_thresh, this->detect_labels, this->detect_alphabet, this->l.classes, 0);

        output_image = image_to_mat(img);

        return {dets, nboxes};
    }


    void ObjectDetector::detect_stream()
    {
        cv::VideoCapture cap(0);
        cv::Mat mat;
        int key;
        while (cap.read(mat) && key != 27)
        {
            cv::Mat detected_img;
            std::pair<detection*, int> dets_nboxes;
            dets_nboxes = this->detect(mat, detected_img);
            cv::imshow("detection", detected_img);
            key = cv::waitKey(1);
        }
    }


    void ObjectDetector::set_thresh(const float &new_thresh)
    {
        this->detect_thresh = new_thresh;
    }

}