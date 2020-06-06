// --------------------------
// Riley Network Header File
// --------------------------


// System includes
#include <vector>
#include <iostream>

// OpenNN includes
#include "../opennn/opennn/opennn.h"


#ifndef RILEY_NETWORK_H
#define RILEY_NETWORK_H


// Functions and object types in use
// -- Currently none --

// Set parameters to be used in the network
namespace RileyNetworkParams
{
    const std::size_t INPUT_LAYER_SIZE_ = 500;
    const std::size_t HIDDEN_LAYERS_SIZE_ = 200;
    const std::size_t OUTPUT_LAYER_SIZE_ = 37;
}


// RileyNetwork class to create neural network model
class RileyNetwork
{
public:

    // Constructor
    RileyNetwork(std::size_t input_layer_size, std::size_t hidden_layer_size, std::size_t output_layer_size);
    // Neural Network member attribute
    OpenNN::NeuralNetwork neural_network; // TODO: make const?

private:

    const OpenNN::Vector<std::size_t> architecture;

    // Layer pointers
    OpenNN::ScalingLayer* scaling_layer_pointer;
    OpenNN::ProbabilisticLayer* probabilistic_layer_pointer;

    // Training
    OpenNN::TrainingStrategy training_strategy;

    // Data
    // DataSet data_set; // TODO: declare in class and assign value in constructor (Error: OpenNN::DataSet does not provide a call operator)

};

#endif //RILEY_NETWORK_H
