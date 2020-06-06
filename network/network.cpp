// -------------------------
// Riley Network Source File
// -------------------------


// System includes
// -- Currently none --
// Included in network.h

// Header includes
#include "network.h"


// Namespaces in use
//using namespace OpenNN;
using namespace RileyNetworkParams;


// RileyNetwork class constructor
RileyNetwork::RileyNetwork(const std::size_t input_layer_size, const std::size_t hidden_layer_size, const std::size_t output_layer_size)
{

    // Print settings
    std::cout << "|------------------------------------------------------------------------------------------------|" << std::endl
              << "| -- CREATING NEURAL NETWORK--                                                                   |" << std::endl
              << "| TYPE            - Classification                                                               |" << std::endl
              << "| LAYERS          - Scaling Layer, Perceptron Layer, Perceptron Layer, ..., Probabilistic Layer  |" << std::endl
              << "|                 - (number of perceptron layers depends of hidden_layer_size)                   |" << std::endl
              << "|                 - Scaling Method                          - No Scaling                         |" << std::endl
              << "|                 - Probabilistic Layer Activation Function - Softmax                            |" << std::endl
              << "| TRAINING METHOD - Loss         - Weighted Squared Error                                        |" << std::endl
              << "|                 - Optimisation - Quasi Newton                                                  |" << std::endl
              << "|------------------------------------------------------------------------------------------------|" << std::endl;


    // --------------
    // Setup the data
    // --------------

    // TODO: setup the data to use in training and testing
    // TODO: get descriptives to add to scaling layer

    // ---------------------------
    // Get and manipulate the data
    // ---------------------------

    // Get the data from ../data/?
    OpenNN::DataSet data_set("../data/", ',', false); // TODO: set path to data

    // Assign names of inputs and targets
    const OpenNN::Vector<std::string> inputs_names = data_set.get_input_variables_names();
    const OpenNN::Vector<std::string> targets_names = data_set.get_target_variables_names();

    // Sort the data randomly into test and training data
    data_set.split_instances_random();

    // Scale the data and obtain the statistical descriptives of each input
    // I.e. max, min, mean, standard deviation
    const OpenNN::Vector<OpenNN::Descriptives> inputs_descriptives = data_set.scale_inputs_minimum_maximum();


    // ---------------------
    // Create Neural Network
    // ---------------------

    // Create a std::vector of the architecture of the neural network
    architecture = {input_layer_size,
                    hidden_layer_size,
                    output_layer_size};

    // Create the neural network as a Classification type
    // Has a scaling layer, two perceptron layers and one probabilistic layer
    neural_network = OpenNN::NeuralNetwork(OpenNN::NeuralNetwork::Classification, architecture);

    // Set scaling layer method
    // Currently has no method
    scaling_layer_pointer = neural_network.get_scaling_layer_pointer();
    scaling_layer_pointer->set_scaling_methods(OpenNN::ScalingLayer::ScalingMethod::NoScaling); // TODO: set appropriate scaling method

    // Set probabilistic layer activation function
    probabilistic_layer_pointer = neural_network.get_probabilistic_layer_pointer();
    probabilistic_layer_pointer->set_activation_function(OpenNN::ProbabilisticLayer::ActivationFunction::Softmax); // TODO: set appropriate activation function


    // ------------------
    // Setup the training TODO: choose the best loss and optimisation methods
    // ------------------

    // Chose and assign the training strategy loss method and optimisation method
    training_strategy(neural_network&, data_set&); // TODO: find and fix the issue here
    training_strategy.set_loss_method(OpenNN::TrainingStrategy::WEIGHTED_SQUARED_ERROR);
    training_strategy.set_optimization_method(OpenNN::TrainingStrategy::QUASI_NEWTON_METHOD);
    OpenNN::QuasiNewtonMethod* quasi_Newton_method_pointer = training_strategy.get_quasi_Newton_method_pointer();
    quasi_Newton_method_pointer->set_minimum_loss_decrease(1.0e-6);

    // Set the info during training to be displayed
    training_strategy.set_display(true);

    // Perform the training
    training_strategy.perform_training();

}


int main()
{
    RileyNetwork riley(INPUT_LAYER_SIZE_,
                       HIDDEN_LAYERS_SIZE_,
                       OUTPUT_LAYER_SIZE_);

    riley.neural_network.get_information();

    return 0;
}

