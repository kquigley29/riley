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


typedef struct {
    double x, y;
    int id;
} centroid;


class CentroidTracker {
public:
    explicit CentroidTracker(int);
    ~CentroidTracker();

    void register_object(int, int);
    void deregister_object(int);
    std::vector<std::pair<int, std::pair<int, int>>> update(const std::vector<std::vector<int>>&);

    std::vector<std::pair<int, std::pair<int, int>>> objects; // < id, <x, y> >
    std::map<int, std::vector<std::pair<int, int>>> path_keeper;
private:
    int max_disappeared;
    int next_object_id;
    std::map<int, int> disappeared;

    static double calc_distance(double, double, double, double);
};


#endif //RILEY_CENTROIDTRACKER_H
