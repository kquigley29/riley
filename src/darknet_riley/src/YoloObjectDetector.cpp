// =========================
// YoloObjectDetector Source
// Detect objects. Fast
// =========================


#include "../include/YoloObjectDetector.h"


YoloObjectDetector::YoloObjectDetector(char *datacfg, char *cfg, char *weights) {
    /*
     YoloObjectDetector uses darknet to detect objects in a video.
     datacfg is the config file containing the location of the class
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
    list *options = read_data_cfg(datacfg);
    detect_classes = option_find_int(options, str_to_char_array("classes"), 20);
    char *label_list = option_find_str(options, str_to_char_array("labels"), str_to_char_array("data/names.list"));
    detect_labels = get_labels(label_list);
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


void *YoloObjectDetector::detect_in_thread(CentroidTracker* ct) {
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
    free_detections(dets, nboxes);

    ct->update(dets, nboxes);
    ct->feynman();
    ct->bohr();

    detect_index = (detect_index + 1)%detect_frame;
    running = 0;
    return nullptr;
}


void *YoloObjectDetector::display_in_thread() {
    /*
     Displays the images produced by YoloObjectDetector::detect_in_thread
     */
    int c = show_image(buff[(buff_index + 1)%3], "Riley Detect", 1);
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


void *YoloObjectDetector::detect(char *video_loc) {
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

    cap = open_video_stream(video_loc);
    if(!cap) error("Couldn't connect to video source.\n");

    buff[0] = get_image_from_stream(cap);
    buff[1] = copy_image(buff[0]);
    buff[2] = copy_image(buff[0]);
    buff_letter[0] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[1] = letterbox_image(buff[0], net->w, net->h);
    buff_letter[2] = letterbox_image(buff[0], net->w, net->h);

    auto *tracker = new CentroidTracker(50);

    int count = 0;
    detect_time = what_time_is_it_now();

    while(!detect_done) {

        buff_index = (buff_index + 1) %3;

        fetch_thread = std::thread(&YoloObjectDetector::fetch_in_thread, this);
        detect_thread = std::thread(&YoloObjectDetector::detect_in_thread, this, tracker);

        fps = 1./(what_time_is_it_now() - detect_time);
        detect_time = what_time_is_it_now();
        display_in_thread();

        fetch_thread.join();
        detect_thread.join();

        ++count;
    }
}


YoloObjectDetector::~YoloObjectDetector() = default;


void *YoloObjectDetector::open_video_stream(char *url) {
    /*
     Opens the video stream from a url or file name.
     Used to fetch video frames inYoloObjectDetector::fetch_in_thread.
     */
    cv::VideoCapture *cap;
    cap = new cv::VideoCapture(url);
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


image YoloObjectDetector::mat_to_image(const cv::Mat& m) {
    /*
     Converts an OpenCV Mat to a darknet image.
     */
    int h = m.rows;
    int w = m.cols;
    int c = m.channels();
    image im = make_image(w, h, c);
    auto *data = (unsigned char*)m.data;
    int step = m.step;
    int i, j, k;

    for(i = 0; i < h; ++i){
        for(k= 0; k < c; ++k){
            for(j = 0; j < w; ++j){
                im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;
            }
        }
    }
    rgbgr_image(im);
    return im;
}


char *YoloObjectDetector::str_to_char_array(const std::string& str) {
    /*
     Converts a standard string to a character array.
     */
    int n = str.length();
    char *char_array = (char*) malloc((n + 1) * sizeof(char));
    std::strcpy(char_array, str.c_str());
    return char_array;
}


void help() {
    /*
     Prints help.
     To be used in the main function to control command line
         arguments.
     */
    std::cout << "[Usage]\n"
              << "./yolo <data cgf> <cfg> <weights> <video or url>\n";
}

int main(int argc, char **argv) {
    /*
     Begins the detection with the given configuration.
     */
    if (argc != 5) {help(); return 1;}
    char *data_cfg_file = argv[1];
    char *cfg_file = argv[2];
    char *weight_file = argv[3];
    char *video = argv[4];

    YoloObjectDetector detector(data_cfg_file, cfg_file, weight_file);
    detector.detect(video);
}