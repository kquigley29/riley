// =========================
// ObjectDetector Source
// Detect objects. Fast
// =========================


#include <iostream>
#include "ObjectDetector.h"
#include "riley_utils.h"


void ObjectDetector::init(char *data_cfg, char *cfg, char *weights, const int &tracker_limit)
{
    /*
     * Used in the constructors to initialise members.
     */
    this->net = load_network(cfg, weights, 0);
    this->detect_alphabet = load_alphabet();
    list *options = read_data_cfg(data_cfg);
    char *label_list = option_find_str(options, str_to_char_array("labels"), str_to_char_array("data/names.list"));
    this->detect_labels = get_labels(label_list);
    set_batch_network(this->net, 1);
    this->l = this->net->layers[this->net->n - 1];
    this->tracker = new CentroidTracker(tracker_limit);
}


ObjectDetector::ObjectDetector(char *data_cfg, char *cfg, char *weights, const int &index, const int &tracker_limit) {
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
    this->init(data_cfg, cfg, weights, tracker_limit);

    std::cout << "Opening video stream...\n";
    this->cap = open_video_stream_from_camera(index);
    if (!this->cap) error("Couldn't connect to video source.\n");
}


ObjectDetector::ObjectDetector(char *data_cfg, char *cfg, char *weights, const char *video, const int &tracker_limit) {
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
    this->init(data_cfg, cfg, weights, tracker_limit);

    std::cout << "Opening video stream...\n";
    this->cap = open_video_stream_from_link(video);
    if (!this->cap) error("Couldn't connect to video source.\n");
}


void ObjectDetector::detect() {
    /*
     Detects objects in images from this->cap.
     */
    this->im = retrieve_image_from_stream(this->cap);
    image sized_im = letterbox_image(this->im, this->net->w, this->net->h);

    float *X = sized_im.data;
    network_predict(this->net, X);
    this->nboxes = 0;
    this->dets = get_network_boxes(this->net, this->im.w, this->im.h, this->detect_thresh, this->detect_hier, 0, 1, &this->nboxes);
    if (this->detect_nms != 0) do_nms_sort(this->dets, this->nboxes, this->l.classes, this->detect_nms);
    draw_detections(this->im, this->dets, this->nboxes, this->detect_thresh, this->detect_labels, this->detect_alphabet, l.classes);

    if (this->track) {
        this->tracker->draw_trace(this->im);
        this->tracker->update(this->dets, this->nboxes);
    }

    free_detections(this->dets, nboxes);

    cv::Mat mat = image_to_mat(this->im);
    free_image(this->im);
    free_image(sized_im);
    this->img = mat;
}


void ObjectDetector::display() {
    while (true) {
        this->detect();
        cv::imshow("detection", this->img);
        if (cv::waitKey(1) == 27) break;
    }
}


cv::Mat ObjectDetector::get_img() const {
    if (!this->img.empty()) return this->img;
    return cv::Mat();
}


detection *ObjectDetector::get_dets() const {
    return this->dets;
}


int ObjectDetector::get_nboxes() const {
    return this->nboxes;
}


void ObjectDetector::set_tracking(const bool &track) {
    this->track = track;
}


void ObjectDetector::update_detect_thresh(const float &new_thresh) {
    this->detect_thresh = new_thresh;
}


ObjectDetector::~ObjectDetector() = default;
