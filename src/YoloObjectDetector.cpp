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
    std::cout << "guys, one of my beehives died :-(\n"
              << "                    ~ pjreddie\n";

    this->net = load_network(cfg, weights, 0);

    std::cout << "NET SET\n";

    this->detect_total = size_network(this->net);

    set_batch_network(net,1);
    this->detect_alphabet = load_alphabet();
    list *options = read_data_cfg(data_cfg);
    this->detect_classes = option_find_int(options, str_to_char_array("classes"), 20);
    char *label_list = option_find_str(options, str_to_char_array("labels"), str_to_char_array("data/names.list"));
    this->detect_labels = get_labels(label_list);

    this->tracker = new CentroidTracker(tracker_limit);
}


void YoloObjectDetector::remember_network(network *net) {
    /*
     Saves the network passed as the argument to memory.
     */
    int count = 0;
    for(int i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        if(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(this->predictions[this->detect_index] + count, net->layers[i].output, sizeof(float) * l.outputs);
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
    fill_cpu(this->detect_total, 0, this->avg, 1);
    for(j = 0; j < this->detect_frame; ++j){
        axpy_cpu(this->detect_total, 1./this->detect_frame, this->predictions[j], 1, this->avg, 1);
    }
    for(i = 0; i < net->n; ++i){
        layer l = net->layers[i];
        if(l.type == YOLO || l.type == REGION || l.type == DETECTION){
            memcpy(l.output, this->avg + count, sizeof(float) * l.outputs);
            count += l.outputs;
        }
    }
    detection *dets = get_network_boxes(net, this->buff[0].w, this->buff[0].h, this->detect_thresh, this->detect_hier, 0, 1, nboxes);
    return dets;
}


void *YoloObjectDetector::fetch_in_thread() {
    /*
     Fetches a video frame in the thread created in the YoloObjectDetector::thread
         function.
     */
    free_image(this->buff[this->buff_index]);
    this->buff[this->buff_index] = retrieve_image_from_stream(this->cap);
    if (this->buff[this->buff_index].data == nullptr) {
        this->detect_done = 1;
        return nullptr;
    }
    letterbox_image_into(this->buff[this->buff_index], this->net->w, this->net->h, this->buff_letter[this->buff_index]);
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
    this->running = 1;
    float nms = .4;

    layer l = this->net->layers[this->net->n-1];
    float *X = this->buff_letter[(this->buff_index+2)%3].data;
    network_predict(this->net, X);

    remember_network(this->net);
    detection *dets = nullptr;
    int nboxes = 0;
    dets = avg_predictions(this->net, &nboxes);

    if (nms > 0) do_nms_obj(dets, nboxes, l.classes, nms);

    printf("\033[2J");
    printf("\033[1;1H");
    printf("\nFPS:%.1f\n",this->fps);
    printf("Objects:\n\n");
    image display = this->buff[(this->buff_index+2) % 3];
    draw_detections(display, dets, nboxes, this->detect_thresh, this->detect_labels, this->detect_alphabet, this->detect_classes);
    this->tracker->draw_trace(display);
    this->tracker->update(dets, nboxes);
    this->tracker->bohr(display.w, display.h);
    this->tracker->feynman(display.w, display.h);
    free_detections(dets, nboxes);

    this->detect_index = (this->detect_index + 1)%this->detect_frame;
    this->running = 0;
    return nullptr;
}


void *YoloObjectDetector::display_in_thread() {
    /*
     Displays the images produced by YoloObjectDetector::detect_in_thread
     */
    int c = show_image(this->buff[(this->buff_index + 1)%3], "Riley", 1);
    if (c != -1) c = c%256;
    if (c == 27) {
        this->detect_done = 1;
        return nullptr;
    } else if (c == 82) {
        this->detect_thresh += .02;
    } else if (c == 84) {
        this->detect_thresh -= .02;
        if(this->detect_thresh <= .02) this->detect_thresh = .02;
    } else if (c == 83) {
        this->detect_hier += .02;
    } else if (c == 81) {
        this->detect_hier -= .02;
        if(this->detect_hier <= .0) this->detect_hier = .0;
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
    set_batch_network(this->net, 1);
    std::thread detect_thread;
    std::thread fetch_thread;

    srand(2222222);

    this->detect_total = size_network(this->net);
    this->predictions = (float**) calloc(this->detect_frame, sizeof(float*));
    for (int i = 0; i < this->detect_frame; ++i){
        this->predictions[i] = (float*) calloc(this->detect_total, sizeof(float));
    }
    this->avg = (float*) calloc(this->detect_total, sizeof(float));

    std::cout << "Open video stream\n";

    if (index >= 0) {
        this->cap = open_video_stream_from_camera(index);
    }
    else {
        this->cap = open_video_stream_from_link(video);
    }
    if(!this->cap) error("Couldn't connect to video source.\n");

    this->buff[0] = retrieve_image_from_stream(this->cap);
    this->buff[1] = copy_image(this->buff[0]);
    this->buff[2] = copy_image(this->buff[0]);
    this->buff_letter[0] = letterbox_image(this->buff[0], this->net->w, this->net->h);
    this->buff_letter[1] = letterbox_image(this->buff[0], this->net->w, this->net->h);
    this->buff_letter[2] = letterbox_image(this->buff[0], this->net->w, this->net->h);

    int count = 0;
    this->detect_time = what_time_is_it_now();

    while(!this->detect_done) {

        this->buff_index = (this->buff_index + 1) %3;

        fetch_thread = std::thread(&YoloObjectDetector::fetch_in_thread, this);
        detect_thread = std::thread(&YoloObjectDetector::detect_in_thread, this);

        this->fps = 1./(what_time_is_it_now() - this->detect_time);
        this->detect_time = what_time_is_it_now();
        display_in_thread();

        fetch_thread.join();
        detect_thread.join();

        ++count;
    }
}


YoloObjectDetector::~YoloObjectDetector() = default;
