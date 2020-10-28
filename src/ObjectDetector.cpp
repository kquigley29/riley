// =========================
// ObjectDetector Source
// Detect objects. Fast
// =========================


#include "ObjectDetector.h"
#include "riley_utils.h"


ObjectDetector::ObjectDetector(char *data_cfg, char *cfg, char *weights)
{
    /*
     * ObjectDetector uses darknet to detect objects in a video.
     * data_cfg is the config file containing the location of the class
         names list file, numbers of classes and other info.
     * cfg is the network configuration file specifying the network
         architecture.
     * weights is the weights file for the network.

     * This constructor sets up the darknet network and prepares the
         network to make detections of the specified classes.
     */
    this->net = load_network(cfg, weights, 0);
    this->detect_alphabet = load_alphabet();
    list *options = read_data_cfg(data_cfg);
    char *label_list = option_find_str(options, str_to_char_array("labels"), str_to_char_array("data/names.list"));
    this->detect_labels = get_labels(label_list);
    set_batch_network(this->net, 1);
    this->l = this->net->layers[this->net->n - 1];
}

cv::Mat ObjectDetector::detect(const cv::Mat &img) {
    image im = mat_to_image(img);
    image sized_im = letterbox_image(im, this->net->w, this->net->h);

    float *X = sized_im.data;
    network_predict(this->net, X);
    this->nboxes = 0;
    this->dets = get_network_boxes(this->net, im.w, im.h, this->detect_thresh, this->detect_hier, nullptr, 1, &this->nboxes);
    if (this->detect_nms != 0) do_nms_sort(this->dets, this->nboxes, this->l.classes, this->detect_nms);
    draw_detections(im, this->dets, this->nboxes, this->detect_thresh, this->detect_labels, this->detect_alphabet, l.classes);

    free_detections(this->dets, nboxes);

    cv::Mat mat = image_to_mat(im);
    free_image(im);
    free_image(sized_im);

    return mat;
}


void ObjectDetector::detect_from_cap(cv::VideoCapture &cap) {
    cv::Mat mat;
    while (cap.read(mat)) {
        cv::Mat detected_img = this->detect(mat);
        ObjectDetector::display(detected_img);
    }
}


void ObjectDetector::display(const cv::Mat &img) {
    cv::imshow("detection", img);
    cv::waitKey(1);
}


detection *ObjectDetector::get_dets() const {
    return this->dets;
}


int ObjectDetector::get_nboxes() const {
    return this->nboxes;
}


void ObjectDetector::set_thresh(const float &new_thresh) {
    this->detect_thresh = new_thresh;
}


ObjectDetector::~ObjectDetector() = default;
