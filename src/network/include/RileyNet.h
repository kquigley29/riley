// =====================
// Neural Network header
// =====================


#include <iostream>
#include <string>
#include <vector>

#include "../../opennn/opennn/opennn.h"


#ifndef RILEY_NETWORK_H
#define RILEY_NETWORK_H


class RileyNet {
public:
    explicit RileyNet(const std::string&);
    ~RileyNet();

    // Member Functions
    void print_architecture();
    void setup_and_train(const OpenNN::Matrix<double>&);

private:
    OpenNN::NeuralNetwork neural_network;
    std::string network_name;

    // Params
    const OpenNN::Vector<std::size_t> INPUT_DIMS {1, 100, 100};
    const OpenNN::Vector<std::size_t> TARGET_DIMS {37};
    const std::size_t OUTPUT_NUM = 37;

    const OpenNN::Vector<std::size_t> FIRST_CONVOLUTION_FILTER_DIMS = {1, 25, 25};
    const std::size_t FIRST_CONVOLUTION_ROW_STRIDE = 3;
    const std::size_t FIRST_CONVOLUTION_COLUMN_STRIDE = 3;

    const std::size_t FIRST_POOLING_LAYER_ROW_STRIDE = 2;
    const std::size_t FIRST_POOLING_LAYER_COLUMN_STRIDE = 2;
    const std::size_t FIRST_POOLING_LAYER_POOL_HEIGHT = 8;
    const std::size_t FIRST_POOLING_LAYER_POOL_WIDTH = 8;

    const std::size_t PERCEPTRON_NEURON_NUM = 37;
};


#endif //RILEY_NETWORK_H
