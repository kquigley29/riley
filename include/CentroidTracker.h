// ======================
// CentroidTracker Header
// Track objects detected
// ======================


#ifndef RILEY_CENTROIDTRACKER_H
#define RILEY_CENTROIDTRACKER_H


#include <vector>
#include <map>
#include <set>
#include <darknet.h>


class CentroidTracker {
public:
    explicit CentroidTracker(const int &max_disappeared=50);
    ~CentroidTracker();

    void draw_trace(image &img);
    std::vector<std::pair<int, std::pair<double, double>>> update(detection *dets, const int &num_dets);

private:
    void register_object(int cX, int cY);
    void deregister_object(int object_id);

    std::vector<std::pair<int, std::pair<double, double>>> objects;
    std::map<int, std::vector<std::pair<double, double>>> path_keeper;
    std::map<int, int> disappeared;
    int max_disappeared;
    int next_object_id;
    float r = 50;

    static double calc_distance(double, double, double, double);
};


#endif //RILEY_CENTROIDTRACKER_H
