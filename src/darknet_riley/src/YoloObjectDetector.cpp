// =========================
// YoloObjectDetector Source
// Detect objects. Fast
// =========================


#include "../include/YoloObjectDetector.h"


YoloObjectDetector::YoloObjectDetector(char *datacfg, char *cfg, char *weights) {

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

    free_image(buff[buff_index]);
    buff[buff_index] = get_image_from_stream(cap);
    if(buff[buff_index].data == nullptr) {
        detect_done = 1;
        return nullptr;
    }
    letterbox_image_into(buff[buff_index], net->w, net->h, buff_letter[buff_index]);
    return nullptr;
}


void *YoloObjectDetector::detect_in_thread() {

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

    detect_index = (detect_index + 1)%detect_frame;
    running = 0;
    return nullptr;
}


void *YoloObjectDetector::display_in_thread() {

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
    if(!cap) error("Couldn't connect to webcam.\n");

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


double *YoloObjectDetector::feynman(detection *dets) {}


YoloObjectDetector::~YoloObjectDetector() = default;


// Helper functions similar to those in darknet/src/image_opencv.cpp
void *YoloObjectDetector::open_video_stream(char *url) {
    cv::VideoCapture *cap;
    cap = new cv::VideoCapture(url);
    if (!cap->isOpened()) return nullptr;
    else return (void *)cap;
}


image YoloObjectDetector::get_image_from_stream(void *p) {
    auto *cap = (cv::VideoCapture *)p;
    cv::Mat m;
    *cap >> m;
    if(m.empty()) return make_empty_image(0,0,0);
    return mat_to_image(m);
}


void YoloObjectDetector::make_window(char *name, int width, int height) {
    cv::namedWindow(name, cv::WINDOW_NORMAL);
    cv::resizeWindow(name, width, height);
}


image mat_to_image(const cv::Mat& m) {
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


char *str_to_char_array(const std::string& str) {

    int n = str.length();
    char *char_array = (char*) malloc((n + 1) * sizeof(char));
    std::strcpy(char_array, str.c_str());
    return char_array;
}


void help() {

    std::cout << "[Usage]\n"
              << "./yolo <data cgf> <cfg> <weights> <video or url>\n";
}

int main(int argc, char **argv) {

    if (argc != 5) {help(); return 1;}
    char *data_cfg_file = argv[1];
    char *cfg_file = argv[2];
    char *weight_file = argv[3];
    char *video = argv[4];

    YoloObjectDetector detector(data_cfg_file, cfg_file, weight_file);
    detector.detect(video);
}