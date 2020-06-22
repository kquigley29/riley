// ======================
// CentroidTracker Source
// Track objects detected
// ======================


#include "../include/CentroidTracker.h"


CentroidTracker::CentroidTracker(int max_disappeared) {

    this->next_object_id = 0;
    this->max_disappeared = max_disappeared;
}


double CentroidTracker::calc_distance(double x1, double y1, double x2, double y2) {

    double x = x1 - x2;
    double y = y1 - y2;
    double dist = sqrt((x * x) + (y * y));

    return dist;
}


void CentroidTracker::register_object(int cX, int cY) {

    int object_id = this->next_object_id;
    this->objects.push_back({object_id, {cX, cY}});
    this->disappeared.insert({object_id, 0});
    this->next_object_id += 1;
}


void CentroidTracker::deregister_object(int object_id) {

    std::cout << "DeRegistered object: " << object_id << "\n";
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

    if (v.size() <= pos) return (v.size());
    std::vector<float>::size_type min = pos;
    for (std::vector<float>::size_type i = pos + 1; i < v.size(); i++) {
        if (v[i] < v[min]) min = i;
    }

    return (min);
}


std::vector<std::pair<int, std::pair<int, int>>> CentroidTracker::update(const std::vector<std::vector<int>> &boxes) {

    if (boxes.empty()) {
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

    // initialize an array of input centroids for the current frame
    std::vector<std::pair<int, int>> input_centroids;
    for (auto b : boxes) {
        int cX = int((b[0] + b[2]) / 2.0);
        int cY = int((b[1] + b[3]) / 2.0);
        input_centroids.emplace_back(cX, cY);
    }

    //if we are currently not tracking any objects take the input centroids and register each of them
    if (this->objects.empty()) {
        for (auto i: input_centroids) {
            this->register_object(i.first, i.second);
        }
    }

        // otherwise, there are currently tracking objects so we need to try to match the
        // input centroids to existing object centroids
    else {
        std::vector<int> object_ids;
        std::vector<std::pair<int, int>> object_centroids;
        for (auto object : this->objects) {
            object_ids.push_back(object.first);
            object_centroids.emplace_back(object.second.first, object.second.second);
        }

//        Calculate Distances
        std::vector<std::vector<float>> distances;
        for (auto &object_centroid : object_centroids) {
            std::vector<float> temp_D;
            for (auto &input_centroid : input_centroids) {
                double dist = calc_distance(object_centroid.first, object_centroid.second, input_centroid.first,
                                           input_centroid.second);

                temp_D.push_back(dist);
            }
            distances.push_back(temp_D);
        }

        // load rows and cols
        std::vector<int> cols;
        std::vector<int> rows;

        //find indices for cols
        for (const auto &v: distances) {
            auto temp = findMin(v);
            cols.push_back(temp);
        }

        //rows calculation
        //sort each mat row for rows calculation
        std::vector<std::vector<float>> D_copy;
        for (auto v: distances) {
            sort(v.begin(), v.end());
            D_copy.push_back(v);
        }

        // use cols calc to find rows
        // slice first elem of each column
        std::vector<std::pair<float, int>> temp_rows;
        int k = 0;
        for (auto i: D_copy) {
            temp_rows.emplace_back(i[0], k);
            k++;
        }
        //print sorted indices of temp_rows
        for (auto const &x : temp_rows) {
            rows.push_back(x.second);
        }

        std::set<int> used_rows;
        std::set<int> used_cols;

        //loop over the combination of the (rows, columns) index tuples
        for (int i = 0; i < rows.size(); i++) {
            //if we have already examined either the row or column value before, ignore it
            if (used_rows.count(rows[i]) || used_cols.count(cols[i])) { continue; }
            //otherwise, grab the object ID for the current row, set its new centroid,
            // and reset the disappeared counter
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

        // compute indexes we have NOT examined yet
        std::set<int> obj_rows;
        std::set<int> inp_cols;

        //D.shape[0]
        for (int i = 0; i < object_centroids.size(); i++) {
            obj_rows.insert(i);
        }
        //D.shape[1]
        for (int i = 0; i < input_centroids.size(); i++) {
            inp_cols.insert(i);
        }

        std::set<int> unused_rows;
        std::set<int> unused_cols;

        set_difference(obj_rows.begin(), obj_rows.end(), used_rows.begin(), used_rows.end(),
                       inserter(unused_rows, unused_rows.begin()));
        set_difference(inp_cols.begin(), inp_cols.end(), used_cols.begin(), used_cols.end(),
                       inserter(unused_cols, unused_cols.begin()));


        //If objCentroids > InpCentroids, we need to check and see if some of these objects have potentially disappeared
        if (object_centroids.size() >= input_centroids.size()) {
            // loop over unused row indexes
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
    //loading path tracking points
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
