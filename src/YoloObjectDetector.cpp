// =========================
// YoloObjectDetector Source
// Detect objects. Fast
// =========================


#include <YoloObjectDetector.h>


YoloObjectDetector::YoloObjectDetector(char *data_cfg, char *cfg, char *weights, const int &tracker_limit) {
    /*
     YoloObjectDetector uses darknet to detect objects in a video.
     data_cfg is the config file containing the location of the class
         names list file, numbers of classes and other info.
     cfg is the network configuration file specifying the network
         architecture.
     weights is the weights file for the network.

    This constructor sets up the darknet network and prepares the
         network to make detections of the specified classes.
     */
    std::cout << "guys, one of my beehives died :-(\n";
    std::cout << "                    ~ pjreddie\n";

    net = load_network(cfg, weights, 0);

    std::cout << "NET SET\n";

    detect_total = size_network(net);

    set_batch_network(net,1);
    detect_alphabet = load_alphabet();
    list *options = read_data_cfg(data_cfg);
    detect_classes = option_find_int(options, str_to_char_array("classes"), 20);
    char *label_list = option_find_str(options, str_to_char_array("labels"), str_to_char_array("data/names.list"));
    detect_labels = get_labels(label_list);

    tracker = new CentroidTracker(tracker_limit);
}


int YoloObjectDetector::size_network(network *net) {
    /*
     Returns the size of the network passed as the argument.
     */
    int count = 0;
    for(int i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        if(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            count += l.outputs;
        }
    }
    return count;
}


void YoloObjectDetector::remember_network(network *net) {
    /*
     Saves the network passed as the argument to memory.
     */
    int count = 0;
    for(int i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        if(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(predictions[detect_index] + count, net->layers[i].output, sizeof(float) * l.outputs);
            count += l.outputs;
        }
    }
}


detection *YoloObjectDetector::avg_predictions(network *net, int *nboxes) {
    /*
     Gets the detections made by the network on a video frame.
     Changes the nboxes value to be the number of bounding boxes.
     */
    int i, j;
    int count = 0;
    fill_cpu(detect_total, 0, avg, 1);
    for(j = 0; j < detect_frame; ++j){
        axpy_cpu(detect_total, 1./detect_frame, predictions[j], 1, avg, 1);
    }
    for(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        if(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(l.output, avg + count, sizeof(float) * l.outputs);
            count += l.outputs;
        }
    }
    detection *dets = get_network_boxes(net, buff[0].w, buff[0].h, detect_thresh, detect_hier, 0, 1, nboxes);
    return dets;
}


void *YoloObjectDetector::fetch_in_thread() {
    /*
     Fetches a video frame in the thread created in the YoloObjectDetector::thread
         function.
     */
    free_image(buff[buff_index]);
    buff[buff_index] = get_image_from_stream(cap);
    if (buff[buff_index].data == nullptr) {
        detect_done = 1;
        return nullptr;
    }
    letterbox_image_into(buff[buff_index], net->w, net->h, buff_letter[buff_index]);
    return nullptr;
}


void *YoloObjectDetector::detect_in_thread() {
    /*
     Detects objects in an image stored in the buffer and draws
         the bounding boxes.
     Uses the CentroidTracker class to calculate the angular and
         linear velocities and accelerations of the detected
         objects.
     */
    running = 1;
    float nms = .4;

    layer l = net->layers[net->n-1];
    float *X = buff_letter[(buff_index+2)%3].data;
    network_predict(net, X);

    remember_network(net);
    detection *dets = nullptr;
    int nboxes = 0;
    dets = avg_predictions(net, &nboxes);

    if (nms > 0) do_nms_obj(dets, nboxes, l.classes, nms);

    printf("\033[2J");
    printf("\033[1;1H");
    printf("\nFPS:%.1f\n",fps);
    printf("Objects:\n\n");
    image display = buff[(buff_index+2) % 3];
    draw_detections(display, dets, nboxes, detect_thresh, detect_labels, detect_alphabet, detect_classes);
    tracker->draw_trace(display);
    tracker->update(dets, nboxes);
    tracker->bohr(display.w, display.h);
    tracker->feynman(display.w, display.h);
    free_detections(dets, nboxes);

    detect_index = (detect_index + 1)%detect_frame;
    running = 0;
    return nullptr;
}


void *YoloObjectDetector::display_in_thread() {
    /*
     Displays the images produced by YoloObjectDetector::detect_in_thread
     */
    int c = show_image(buff[(buff_index + 1)%3], "Riley", 1);
    if (c != -1) c = c%256;
    if (c == 27) {
        detect_done = 1;
        return nullptr;
    } else if (c == 82) {
        detect_thresh += .02;
    } else if (c == 84) {
        detect_thresh -= .02;
        if(detect_thresh <= .02) detect_thresh = .02;
    } else if (c == 83) {
        detect_hier += .02;
    } else if (c == 81) {
        detect_hier -= .02;
        if(detect_hier <= .0) detect_hier = .0;
    }
    return nullptr;
}


void *YoloObjectDetector::detect(const char *video, const int &index) {
    /*
     Calls the YoloObjectDetector::detect_in_thread and
          YoloObjectDetector::fetch_in_thread functions
          and runs them in separate threads.
     Initiates the CentroidTracker object.
     */
    set_batch_network(net, 1);
    std::thread detect_thread;
    std::thread fetch_thread;

    srand(2222222);

    detect_total = size_network(net);
    predictions = (float**) calloc(detect_frame, sizeof(float*));
    for (int i = 0; i < detect_frame; ++i){
        predictions[i] = (float*) calloc(detect_total, sizeof(float));
    }
    avg = (float*) calloc(detect_total, sizeof(float));

    std::cout << "Open video stream\n";

    if (index >= 0) {
        cap = open_video_stream_from_camera(index);
    }
    else {
        cap = open_video_stream_from_link(video);
    }
    if(!cap) error("Couldn't connect to video source.\n");

    buff[0] = get_image_from_stream(cap);
    buff[1] = copy_image(buff[0]);
    buff[2] = copy_image(buff[0]);
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);

    int count = 0;
    detect_time = what_time_is_it_now();

    while(!detect_done) {

        buff_index = (buff_index + 1) %3;

        fetch_thread = std::thread(&YoloObjectDetector::fetch_in_thread, this);
        detect_thread = std::thread(&YoloObjectDetector::detect_in_thread, this);

        fps = 1./(what_time_is_it_now() - detect_time);
        detect_time = what_time_is_it_now();
        display_in_thread();

        fetch_thread.join();
        detect_thread.join();

        ++count;
    }
}


YoloObjectDetector::~YoloObjectDetector() = default;


void *YoloObjectDetector::open_video_stream_from_camera(const int &index) {
    /*
     Opens the video stream from a camera.
     Used to fetch video frames in YoloObjectDetector::fetch_in_thread.
     */
    cv::VideoCapture *cap;
    cap = new cv::VideoCapture(index);
    if (!cap->isOpened()) return nullptr;
    else return (void *)cap;
}


void *YoloObjectDetector::open_video_stream_from_link(const char *link) {
    /*
     Opens the video stream from a url or file name.
     Used to fetch video frames in YoloObjectDetector::detect.
     */
    cv::VideoCapture *cap;
    cap = new cv::VideoCapture(link);
    if (!cap->isOpened()) return nullptr;
    else return (void *)cap;
}


image YoloObjectDetector::get_image_from_stream(void *p) {
    /*
     Gets an image from the video stream.
     If the image is empty an empty image is returned from
         the darknet make_empty_image function
     */
    auto *cap = (cv::VideoCapture *)p;
    cv::Mat m;
    *cap >> m;
    if(m.empty()) return make_empty_image(0,0,0);
    return mat_to_image(m);
}
