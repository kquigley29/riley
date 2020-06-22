// ======================
// CentroidTracker Header
// Track objects detected
// ======================


#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <set>
#include <algorithm>
#include <thread>

#include <darknet.h>


#ifndef RILEY_CENTROIDTRACKER_H
#define RILEY_CENTROIDTRACKER_H


class CentroidTracker {
public:
    explicit CentroidTracker(int);
    ~CentroidTracker();

    std::vector<std::pair<int, std::pair<double, double>>> update(detection*, const int&);
    void feynman();
    void bohr();

private:
    void register_object(int, int);
    void deregister_object(int);

    std::vector<std::pair<int, std::pair<double, double>>> objects;
    std::map<int, std::vector<std::pair<double, double>>> path_keeper;
    std::map<int, int> disappeared;
    int max_disappeared;
    int next_object_id;
    float r = 50;

    static double calc_distance(double, double, double, double);
};


#endif //RILEY_CENTROIDTRACKER_H
