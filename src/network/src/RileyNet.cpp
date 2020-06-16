// =====================
// Neural Network Source
// =====================

// Includes
#include "../include/RileyNet.h"


// RileyNet Constructor
// Create the Neural Network with the specified parameters
RileyNet::RileyNet(const std::string& name) {

    std::cout << "R I L E Y  N E U R A L  N E T W O R K\n\n";
    std::cout << "Rule no. 1: Fuck SourceForge.\n";
    std::cout << "Setting up the network...\n";

    network_name = name;

    // Scaling Layer
    auto* scaling_layer_ptr = new OpenNN::ScalingLayer(INPUT_DIMS);
    scaling_layer_ptr->set_scaling_methods(OpenNN::ScalingLayer::ScalingMethod::MinimumMaximum);
    neural_network.add_layer(scaling_layer_ptr);

    const OpenNN::Vector<std::size_t> scaling_layer_output_dims = scaling_layer_ptr->get_outputs_dimensions();

    // First Convolution Layer
    auto* first_convolution_layer_ptr = new OpenNN::ConvolutionalLayer(scaling_layer_output_dims, FIRST_CONVOLUTION_FILTER_DIMS);
    first_convolution_layer_ptr->set_row_stride(FIRST_CONVOLUTION_ROW_STRIDE);
    first_convolution_layer_ptr->set_column_stride(FIRST_CONVOLUTION_COLUMN_STRIDE);
    neural_network.add_layer(first_convolution_layer_ptr);

    const OpenNN::Vector<std::size_t> first_convolution_layer_output_dims = first_convolution_layer_ptr->get_outputs_dimensions();

    // Pooling Layer
    auto* first_pooling_layer_ptr = new OpenNN::PoolingLayer(first_convolution_layer_output_dims);
    first_pooling_layer_ptr->set_pooling_method(OpenNN::PoolingLayer::MaxPooling);
    first_pooling_layer_ptr->set_row_stride(FIRST_POOLING_LAYER_ROW_STRIDE);
    first_pooling_layer_ptr->set_column_stride(FIRST_POOLING_LAYER_COLUMN_STRIDE);
    first_pooling_layer_ptr->set_pool_size(FIRST_POOLING_LAYER_POOL_HEIGHT, FIRST_POOLING_LAYER_POOL_WIDTH);
    neural_network.add_layer(first_pooling_layer_ptr);

    const OpenNN::Vector<std::size_t> first_pooling_layer_output_dims = first_convolution_layer_ptr->get_outputs_dimensions();

    // Perceptron Layer
    std::size_t perceptron_input_size = first_pooling_layer_output_dims.calculate_product();
    auto* perceptron_layer = new OpenNN::PerceptronLayer(perceptron_input_size, PERCEPTRON_NEURON_NUM);
    neural_network.add_layer(perceptron_layer);

    const std::size_t perceptron_layer_outputs = perceptron_layer->get_neurons_number();

    // Probabilistic Layer
    auto* probabilistic_layer = new OpenNN::ProbabilisticLayer(perceptron_layer_outputs, OUTPUT_NUM);
    neural_network.add_layer(probabilistic_layer);

    neural_network.print_summary();

    std::cout << "Network is set up.\n";
}


// Print the network architecture
void RileyNet::print_architecture() {

    std::cout << "Architecture: " << neural_network.get_architecture() << "\n";
}


// Set the data as the input matrix and set all training parameters
// Train the model on the data matrix
void RileyNet::setup_and_train(const OpenNN::Matrix<double>&  matrix_data) {


    // Setup Data Set
    // Set the matrix as the data set
    // Assign uses to the columns
    OpenNN::DataSet data_set(matrix_data);
    data_set.set_input();
    for (int i = 0; i < 37; i++) {
        data_set.set_column_use(i, OpenNN::DataSet::VariableUse::Target);
    }

    data_set.set_input_variables_dimensions(INPUT_DIMS);
    data_set.set_target_variables_dimensions(TARGET_DIMS);

    // Keep all the data for training
    data_set.split_instances_sequential(1, 0, 0);

    data_set.set_batch_instances_number(BATCH_INSTANCES_NUMBER);


    // Setup Training
    OpenNN::TrainingStrategy training_strategy(&neural_network, &data_set);
    training_strategy.set_optimization_method(OpenNN::TrainingStrategy::OptimizationMethod::STOCHASTIC_GRADIENT_DESCENT);
    training_strategy.set_loss_method(OpenNN::TrainingStrategy::LossMethod::MEAN_SQUARED_ERROR);
//    training_strategy.get_loss_index_pointer()->set_regularization_method(OpenNN::LossIndex::RegularizationMethod::L2);
    training_strategy.set_display(true);

    OpenNN::StochasticGradientDescent* sgd_pointer = training_strategy.get_stochastic_gradient_descent_pointer();

//    sgd_pointer->set_minimum_loss_increase(1.0e-6);
    sgd_pointer->set_maximum_epochs_number(0);
    sgd_pointer->set_display_period(1);
    sgd_pointer->set_maximum_time(1800);
    sgd_pointer->set_initial_learning_rate(0.1);

    // Train the model
    OpenNN::OptimizationAlgorithm::Results training_strategy_results = training_strategy.perform_training();


    // Training Analysis
    OpenNN::TestingAnalysis testing_analysis(&neural_network, &data_set);

    OpenNN::Matrix<size_t> confusion = testing_analysis.calculate_confusion();

//    cout << "\n\nConfusion matrix: \n" << "\n" << confusion << "\n";
//    cout << "\nAccuracy: " << (confusion.calculate_trace()/confusion.calculate_sum())*100 << " %" << "\n\n";

    neural_network.save("/home/keane/Projects/riley/src/network/network_meta/store/" + network_name + "_network.xml");
    training_strategy.save("/home/keane/Projects/riley/src/network/network_meta/store/" + network_name + "_train_strat.xml");
}

RileyNet::~RileyNet() = default;