# Root CMake file
cmake_minimum_required(VERSION 3.15)
project(riley)
set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
set(OpenNN_DIR ${CMAKE_SOURCE_DIR}/src/opennn/opennn)
set(Darknet_DIR ${CMAKE_SOURCE_DIR}/src/darknet)

# Set all binaries to be placed in riley/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Include all the include directories
include_directories(
        ${CMAKE_SOURCE_DIR}/src/manipulate/include
        ${CMAKE_SOURCE_DIR}/src/network/include
        ${OpenNN_DIR}
        ${Darknet_DIR}/include
        ${Darknet_DIR}/src)

# Add the manipulate library
add_library(manipulate_lib
        ${CMAKE_SOURCE_DIR}/src/manipulate/src/VideoProcessor.cpp
        ${CMAKE_SOURCE_DIR}/src/manipulate/src/ImageProcessor.cpp)
target_link_libraries(manipulate_lib
        ${OpenCV_LIBS})

# Add the network library
link_directories(
        ${OpenNN_DIR})
add_subdirectory(${OpenNN_DIR})
add_library(network_lib
        ${CMAKE_SOURCE_DIR}/src/network/src/RileyNet.cpp)
target_link_libraries(network_lib
        opennn_lib)


# Add the darknet library
add_library(darknet_riley_lib
        ${CMAKE_SOURCE_DIR}/src/darknet_riley/src/YoloObjectDetector.cpp

        ${Darknet_DIR}/src/activation_layer.c        ${Darknet_DIR}/src/im2col.c
        ${Darknet_DIR}/src/activations.c             ${Darknet_DIR}/src/image.c
        ${Darknet_DIR}/src/avgpool_layer.c           ${Darknet_DIR}/src/layer.c
        ${Darknet_DIR}/src/batchnorm_layer.c         ${Darknet_DIR}/src/list.c
        ${Darknet_DIR}/src/blas.c                    ${Darknet_DIR}/src/local_layer.c
        ${Darknet_DIR}/src/box.c                     ${Darknet_DIR}/src/lstm_layer.c
        ${Darknet_DIR}/src/col2im.c                  ${Darknet_DIR}/src/matrix.c
        ${Darknet_DIR}/src/connected_layer.c         ${Darknet_DIR}/src/maxpool_layer.c
        ${Darknet_DIR}/src/convolutional_layer.c     ${Darknet_DIR}/src/network.c
        ${Darknet_DIR}/src/cost_layer.c              ${Darknet_DIR}/src/normalization_layer.c
        ${Darknet_DIR}/src/image_opencv.cpp	         ${Darknet_DIR}/src/iseg_layer.c
        ${Darknet_DIR}/src/crnn_layer.c              ${Darknet_DIR}/src/option_list.c
        ${Darknet_DIR}/src/crop_layer.c              ${Darknet_DIR}/src/parser.c
        ${Darknet_DIR}/src/cuda.c                    ${Darknet_DIR}/src/region_layer.c
        ${Darknet_DIR}/src/data.c                    ${Darknet_DIR}/src/reorg_layer.c
        ${Darknet_DIR}/src/deconvolutional_layer.c   ${Darknet_DIR}/src/rnn_layer.c
        ${Darknet_DIR}/src/demo.c                    ${Darknet_DIR}/src/route_layer.c
        ${Darknet_DIR}/src/detection_layer.c         ${Darknet_DIR}/src/shortcut_layer.c
        ${Darknet_DIR}/src/dropout_layer.c           ${Darknet_DIR}/src/softmax_layer.c
        ${Darknet_DIR}/src/gemm.c                    ${Darknet_DIR}/src/tree.c
        ${Darknet_DIR}/src/gru_layer.c               ${Darknet_DIR}/src/utils.c
        ${Darknet_DIR}/src/upsample_layer.c          ${Darknet_DIR}/src/logistic_layer.c
        ${Darknet_DIR}/src/l2norm_layer.c            ${Darknet_DIR}/src/yolo_layer.c

        ${Darknet_DIR}/src/activation_kernels.cu     ${Darknet_DIR}/src/crop_layer_kernels.cu
        ${Darknet_DIR}/src/avgpool_layer_kernels.cu  ${Darknet_DIR}/src/deconvolutional_kernels.cu
        ${Darknet_DIR}/src/blas_kernels.cu           ${Darknet_DIR}/src/dropout_layer_kernels.cu
        ${Darknet_DIR}/src/col2im_kernels.cu         ${Darknet_DIR}/src/im2col_kernels.cu
        ${Darknet_DIR}/src/convolutional_kernels.cu  ${Darknet_DIR}/src/maxpool_layer_kernels.cu)
target_link_libraries(darknet_riley_lib
        ${OpenCV_LIBs})



        # Add the riley executable
add_executable(riley
        ${CMAKE_SOURCE_DIR}/src/riley.cpp)
target_link_libraries(riley
        manipulate_lib
        network_lib
        darknet_riley_lib)

# Add the manipulation executable
add_executable(manipulate
        ${CMAKE_SOURCE_DIR}/src/manipulate/src/manipulate.cpp)
target_link_libraries(manipulate
        manipulate_lib)

