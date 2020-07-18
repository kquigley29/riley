// ======================
// CentroidTracker Source
// Track objects detected
// ======================


#include "CentroidTracker.h"


CentroidTracker::CentroidTracker(int max_disappeared) {
    /*
     CentroidTracker tracks object centroids passed it as darknet detections.
     The objects are tracked by recording the path taken by each objects. An
          object is assumed to be the same object in consecutive frames in
          the basis that objects will not move extreme distances within the
          time of one frame passing. Therefore the closest pairs, one from
          each frame, are taken to be the same object.
     Objects are forgotten once they have been out of sight for longer than
          a consecutive number of frames given by max_disappeared.
     */
    std::cout << "Onwards and upwards.";

    this->next_object_id = 0;
    this->max_disappeared = max_disappeared;
}


double CentroidTracker::calc_distance(double x1, double y1, double x2, double y2) {
    /*
     Calculate the euclidean distance between two points in the plane of
         the image.
     */
    double x = x1 - x2;
    double y = y1 - y2;
    double dist = sqrt((x * x) + (y * y));

    return dist;
}


void CentroidTracker::register_object(int cX, int cY) {
    /*
     Register a centroid for object tracking.
     */
    int object_id = this->next_object_id;
    this->objects.push_back({object_id, {cX, cY}});
    this->disappeared.insert({object_id, 0});
    this->next_object_id += 1;
}


void CentroidTracker::deregister_object(int object_id) {
    /*
     Deregister a lost centroid from object tracking.
     */
    std::cout << "Deregistered object: " << object_id << "\n";
    if (!this->objects.empty()) {
        for (int i = 0; i < this->objects.size(); i++) {
            if (this->objects[i].first == object_id) {
                this->objects.erase(this->objects.begin() + i);
            }
        }
        this->disappeared.erase(object_id);
        this->path_keeper.erase(object_id);
    }
}


std::vector<float>::size_type findMin(const std::vector<float> &v, std::vector<float>::size_type pos = 0) {
    /*
     Find the minimum value in a vector.
     */
    if (v.size() <= pos) return (v.size());
    std::vector<float>::size_type min = pos;
    for (std::vector<float>::size_type i = pos + 1; i < v.size(); i++) {
        if (v[i] < v[min]) min = i;
    }

    return (min);
}


void CentroidTracker::draw_trace(image &img) {
    /*
     Plot the points contained in path_keeper.
     Traces a line showing the movement of all centroids.
     */
    if (!objects.empty()){
        for (auto obj: objects) {
            auto coordinates = path_keeper[obj.first];
            for (auto coordinate: coordinates) {
                int width = 4;
                int x1 = coordinate.first*img.w - width/2;
                int y1 = coordinate.second*img.h - width/2;
                int x2 = coordinate.first*img.w + width/2;
                int y2 = coordinate.second*img.h + width/2;
                draw_box_width(img, x1, y1, x2, y2, width, 1, 0, 0);
            }
        }
    }
}


void CentroidTracker::feynman(const int &w, const int &h) {
    /*
     Calculate the angular velocity and acceleration of a tracked object.
     Done in the image plane and therefore unreliable if the object
          movements are not orthogonal to the lense.
     */
    if (!objects.empty()) {
        for (auto obj : objects) {
            std::vector<std::pair<double, double>> positions = path_keeper[obj.first];
            int n = positions.size();
            if (n >= 5) {

                auto p1 = positions[n-1];
                auto p2 = positions[n-3];
                auto p3 = positions[n-5];

                double s1 = calc_distance(p1.first*w, p1.second*h, p2.first*w, p2.second*h);
                double s2 = calc_distance(p2.first*w, p2.second*h, p3.first*w, p3.second*h);

                double omega1 = s1 / r;
                double omega2 = s2 / r;

                double omega = (omega2 + omega1) / 2.;
                double alpha = (omega2 - omega1) / 2.;

                if (omega != 0 || alpha != 0) {
                    std::cout << "Object " << obj.first << " omega: " << omega << "\n";
                    std::cout << "Object " << obj.first << " alpha: " << alpha << "\n";
                }
            }
        }
    }
}


void CentroidTracker::bohr(const int &w, const int &h) {
    /*
     Calculate the linear velocity and acceleration of a tracked object.
     Done in the image plane and therefore unreliable if the object
          movements are not orthogonal to the lense.
     */
    if (!objects.empty()) {
        for (auto obj : objects) {
            std::vector<std::pair<double, double>> positions = path_keeper[obj.first];
            int n = positions.size();
            if (n >= 9) {

                auto p1 = positions[n-1];
                auto p2 = positions[n-5];
                auto p3 = positions[n-9];

                double v1 = calc_distance(p1.first*w, p1.second*h, p2.first*w, p2.second*h);
                double v2 = calc_distance(p2.first*w, p2.second*h, p3.first*w, p3.second*h);

                double v = (v2 + v1) / 2;
                double a = (v2 - v1) / 2;

                if (v >= 0.05 || a >= 0.05) {
                    std::cout << "Object " << obj.first << " v: " << v << "\n";
                    std::cout << "Object " << obj.first << " a: " << a << "\n";
                }
            }
        }
    }
}


std::vector<std::pair<int, std::pair<double, double>>> CentroidTracker::update(detection *dets, const int &num_dets) {
    /*
     Updates the positions of the detected objects and records if an object has
         disappeared from view.
     Records the path of all objects being tracked.
     */
    std::cout << "Number of detections: " << num_dets << "\n";

    if (num_dets == 0) {
        if (!this->disappeared.empty()) {
            for (auto elem : this->disappeared) {
                this->disappeared[elem.first]++;

                if (elem.second > this->max_disappeared) {
                    this->deregister_object(elem.first);
                }
            }
        }
        return this->objects;
    }

    std::vector<std::pair<double, double>> input_centroids;
    for (int i = 0; i != num_dets; i++) {
        float cX = dets[i].bbox.x;
        float cY = dets[i].bbox.y;
        input_centroids.emplace_back(cX, cY);
    }

    if (this->objects.empty()) {
        for (auto i: input_centroids) {
            this->register_object(i.first, i.second);
        }
    }
    else {
        std::vector<int> object_ids;
        std::vector<std::pair<double, double>> object_centroids;
        for (auto object : this->objects) {
            object_ids.push_back(object.first);
            object_centroids.emplace_back(object.second.first, object.second.second);
        }

        std::vector<std::vector<float>> distances;
        for (auto &object_centroid : object_centroids) {
            std::vector<float> temp_D;
            for (auto &input_centroid : input_centroids) {
                double dist = calc_distance(object_centroid.first, object_centroid.second,
                                            input_centroid.first, input_centroid.second);

                temp_D.push_back(dist);
            }
            distances.push_back(temp_D);
        }

        std::vector<int> cols;
        std::vector<int> rows;

        for (const auto &v: distances) {
            auto temp = findMin(v);
            cols.push_back(temp);
        }

        std::vector<std::vector<float>> D_copy;
        for (auto v: distances) {
            sort(v.begin(), v.end());
            D_copy.push_back(v);
        }

        std::vector<std::pair<float, int>> temp_rows;
        int k = 0;
        for (auto i: D_copy) {
            temp_rows.emplace_back(i[0], k);
            k++;
        }

        for (auto const &x : temp_rows) {
            rows.push_back(x.second);
        }

        std::set<int> used_rows;
        std::set<int> used_cols;

        for (int i = 0; i < rows.size(); i++) {
            if (used_rows.count(rows[i]) || used_cols.count(cols[i])) { continue; }
            int object_id = object_ids[rows[i]];
            for (auto & object : this->objects) {
                if (object.first == object_id) {
                    object.second.first = input_centroids[cols[i]].first;
                    object.second.second = input_centroids[cols[i]].second;
                }
            }
            this->disappeared[object_id] = 0;

            used_rows.insert(rows[i]);
            used_cols.insert(cols[i]);
        }

        std::set<int> obj_rows;
        std::set<int> inp_cols;

        for (int i = 0; i < object_centroids.size(); i++) {
            obj_rows.insert(i);
        }
        for (int i = 0; i < input_centroids.size(); i++) {
            inp_cols.insert(i);
        }

        std::set<int> unused_rows;
        std::set<int> unused_cols;

        set_difference(obj_rows.begin(), obj_rows.end(), used_rows.begin(), used_rows.end(),
                       inserter(unused_rows, unused_rows.begin()));
        set_difference(inp_cols.begin(), inp_cols.end(), used_cols.begin(), used_cols.end(),
                       inserter(unused_cols, unused_cols.begin()));


        if (object_centroids.size() >= input_centroids.size()) {
            for (auto row: unused_rows) {
                int object_id = object_ids[row];
                this->disappeared[object_id] += 1;

                if (this->disappeared[object_id] > this->max_disappeared) {
                    this->deregister_object(object_id);
                }
            }
        } else {
            for (auto col: unused_cols) {
                this->register_object(input_centroids[col].first, input_centroids[col].second);
            }
        }
    }
    if (!objects.empty()) {
        for (auto obj: objects) {

            if (path_keeper[obj.first].size() > 30) {
                path_keeper[obj.first].erase(path_keeper[obj.first].begin());
            }
            path_keeper[obj.first].push_back(std::make_pair(obj.second.first, obj.second.second));
        }
    }
    return this->objects;
}


CentroidTracker::~CentroidTracker() = default;
