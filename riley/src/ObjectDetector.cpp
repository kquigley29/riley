// =========================
// ObjectDetector Source
// Detect objects. Fast
// =========================


#include <ObjectDetector.h>


ObjectDetector::ObjectDetector(char *data_cfg, char *cfg, char *weights, const int &index, const int &tracker_limit)
: net(load_network(cfg, weights, 0))
, detect_total(size_network(this->net))
, detect_alphabet(load_alphabet())
, tracker(new CentroidTracker(tracker_limit))
{
    /*
     ObjectDetector uses darknet to detect objects in a video.
     data_cfg is the config file containing the location of the class
         names list file, numbers of classes and other info.
     cfg is the network configuration file specifying the network
         architecture.
     weights is the weights file for the network.

    This constructor sets up the darknet network and prepares the
         network to make detections of the specified classes.
     */
    std::cout << "guys, one of my beehives died :-(\n"
              << "                    ~ pjreddie\n";

    std::cout << "NET SET\n";

    set_batch_network(net,1);
    list *options = read_data_cfg(data_cfg);
    this->detect_classes = option_find_int(options, str_to_char_array("classes"), 20);
    char *label_list = option_find_str(options, str_to_char_array("labels"), str_to_char_array("data/names.list"));
    this->detect_labels = get_labels(label_list);
    this->get_cap(index);
}


ObjectDetector::ObjectDetector(char *data_cfg, char *cfg, char *weights, const char *video, const int &tracker_limit)
        : net(load_network(cfg, weights, 0))
        , detect_total(size_network(this->net))
        , detect_alphabet(load_alphabet())
        , tracker(new CentroidTracker(tracker_limit))
{
    /*
     ObjectDetector uses darknet to detect objects in a video.
     data_cfg is the config file containing the location of the class
         names list file, numbers of classes and other info.
     cfg is the network configuration file specifying the network
         architecture.
     weights is the weights file for the network.

    This constructor sets up the darknet network and prepares the
         network to make detections of the specified classes.
     */
    std::cout << "guys, one of my beehives died :-(\n"
              << "                    ~ pjreddie\n";

    std::cout << "NET SET\n";

    set_batch_network(net,1);
    list *options = read_data_cfg(data_cfg);
    this->detect_classes = option_find_int(options, str_to_char_array("classes"), 20);
    char *label_list = option_find_str(options, str_to_char_array("labels"), str_to_char_array("data/names.list"));
    this->detect_labels = get_labels(label_list);
    this->get_cap(video);
}


cv::VideoCapture *ObjectDetector::get_cap(const int &index) {
    /*
     Gets a cv::VideoCapture objects to pass images to the detector.
     This function retrieves the image from a camera index.
     */
    set_batch_network(this->net, 1);

    this->detect_total = size_network(this->net);
    this->predictions = (float**) calloc(this->detect_frame, sizeof(float*));

    for (int i = 0; i < this->detect_frame; ++i){
        this->predictions[i] = (float*) calloc(this->detect_total, sizeof(float));
    }

    this->avg = (float*) calloc(this->detect_total, sizeof(float));

    std::cout << "Opening video stream...\n";

    this->cap = open_video_stream_from_camera(index);

    if(!this->cap) error("Couldn't connect to video source.\n");

    return nullptr;
}


cv::VideoCapture *ObjectDetector::get_cap(const char *video) {
    /*
     Gets a cv::VideoCapture objects to pass images to the detector.
     This function retrieves the image from a file path or web link.
     */
    set_batch_network(this->net, 1);

    this->detect_total = size_network(this->net);
    this->predictions = (float**) calloc(this->detect_frame, sizeof(float*));

    for (int i = 0; i < this->detect_frame; ++i){
        this->predictions[i] = (float*) calloc(this->detect_total, sizeof(float));
    }

    this->avg = (float*) calloc(this->detect_total, sizeof(float));

    std::cout << "Opening video stream...\n";

    this->cap = open_video_stream_from_link(video);

    if(!this->cap) error("Couldn't connect to video source.\n");

    return nullptr;
}


void *ObjectDetector::detect(const bool &track) {
    /*
     Detects objects in the image from this->cap.
     */
    if (track) {

    }

    while (true) {
        image im = retrieve_image_from_stream(this->cap);
        image sized_im = letterbox_image(im, this->net->w, this->net->h);
        layer l = this->net->layers[this->net->n - 1];

        float *X = sized_im.data;
        network_predict(this->net, X);
        int nboxes = 0;
        detection *dets = get_network_boxes(this->net, im.w, im.h, this->detect_thresh, this->detect_hier, 0, 1, &nboxes);
        if (this->detect_nms != 0) do_nms_sort(dets, nboxes, l.classes, this->detect_nms);
        draw_detections(im, dets, nboxes, this->detect_thresh, this->detect_labels, this->detect_alphabet, l.classes);

        if (track) {
            this->tracker->draw_trace(im);
            this->tracker->update(dets, nboxes);
        }

        free_detections(dets, nboxes);

        cv::Mat mat = image_to_mat(im);
        cv::imshow("detection", mat);

        if (cv::waitKey(1) == 27) {
            std::cout << "Closed.\n";
            break;
        }
        free_image(im);
        free_image(sized_im);
    }
    return nullptr;
}


ObjectDetector::~ObjectDetector() = default;
