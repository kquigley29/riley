# Root CMake file
cmake_minimum_required(VERSION 3.15)
project(riley)
set(CMAKE_CXX_STANDARD 14)

FIND_PACKAGE(CUDA)
if (CUDA_FOUND)
    find_package(CUDA REQUIRED)
    set(
    CUDA_NVCC_FLAGS
    ${CUDA_NVCC_FLAGS};
    -gencode arch=compute_75,code=sm_75)
    add_definitions(-DGPU)
else()
    list(APPEND LIBRARIES "m")
endif()
find_package(OpenCV REQUIRED)
set(OpenNN_DIR ${CMAKE_SOURCE_DIR}/src/opennn/opennn)
set(Darknet_DIR ${CMAKE_SOURCE_DIR}/src/darknet)

add_definitions(-DOPENCV)

# Set all binaries to be placed in riley
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

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

if (CUDA_FOUND)
    link_directories(/usr/local/cua/lib64)
    # Add the darknet library
    cuda_add_library(darknet_riley_lib
            ${CMAKE_SOURCE_DIR}/src/darknet_riley/src/YoloObjectDetector.cpp
            ${CMAKE_SOURCE_DIR}/src/darknet_riley/src/CentroidTracker.cpp

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
            ${Darknet_DIR}/src/image_opencv.cpp	     ${Darknet_DIR}/src/iseg_layer.c
            ${Darknet_DIR}/src/crnn_layer.c              ${Darknet_DIR}/src/option_list.c
            ${Darknet_DIR}/src/crop_layer.c              ${Darknet_DIR}/src/parser.c
            ${Darknet_DIR}/src/cuda.c                    ${Darknet_DIR}/src/region_layer.c
            ${Darknet_DIR}/src/data.c                    ${Darknet_DIR}/src/reorg_layer.c
            ${Darknet_DIR}/src/deconvolutional_layer.c   ${Darknet_DIR}/src/rnn_layer.c
            ${Darknet_DIR}/src/route_layer.c             ${Darknet_DIR}/src/detection_layer.c
            ${Darknet_DIR}/src/shortcut_layer.c          ${Darknet_DIR}/src/dropout_layer.c
            ${Darknet_DIR}/src/softmax_layer.c           ${Darknet_DIR}/src/gemm.c
            ${Darknet_DIR}/src/tree.c                    ${Darknet_DIR}/src/gru_layer.c
            ${Darknet_DIR}/src/utils.c                   ${Darknet_DIR}/src/upsample_layer.c
            ${Darknet_DIR}/src/logistic_layer.c          ${Darknet_DIR}/src/l2norm_layer.c
            ${Darknet_DIR}/src/yolo_layer.c              ${Darknet_DIR}/src/demo.c

            ${Darknet_DIR}/src/activation_kernels.cu     ${Darknet_DIR}/src/crop_layer_kernels.cu
            ${Darknet_DIR}/src/avgpool_layer_kernels.cu  ${Darknet_DIR}/src/deconvolutional_kernels.cu
            ${Darknet_DIR}/src/blas_kernels.cu           ${Darknet_DIR}/src/dropout_layer_kernels.cu
            ${Darknet_DIR}/src/col2im_kernels.cu         ${Darknet_DIR}/src/im2col_kernels.cu
            ${Darknet_DIR}/src/convolutional_kernels.cu  ${Darknet_DIR}/src/maxpool_layer_kernels.cu)
    target_link_libraries(darknet_riley_lib
            m
            pthread
            stdc++
            cuda
            cudart
            cublas
            curand
            ${OpenCV_LIBS})
    # Add the yolo executable
    cuda_add_executable(yolo
            ${CMAKE_SOURCE_DIR}/src/darknet_riley/src/YoloObjectDetector.cpp)
    target_link_libraries(yolo
            darknet_riley_lib)

else()

    # Add the darknet_riley library
    add_library(darknet_riley_lib
            ${CMAKE_SOURCE_DIR}/src/darknet_riley/src/YoloObjectDetector.cpp
            ${CMAKE_SOURCE_DIR}/src/darknet_riley/src/CentroidTracker.cpp

            ${DARKNET_PATH}/src/activation_layer.c        ${DARKNET_PATH}/src/im2col.c
            ${DARKNET_PATH}/src/activations.c             ${DARKNET_PATH}/src/image.c
            ${DARKNET_PATH}/src/avgpool_layer.c           ${DARKNET_PATH}/src/layer.c
            ${DARKNET_PATH}/src/batchnorm_layer.c         ${DARKNET_PATH}/src/list.c
            ${DARKNET_PATH}/src/blas.c                    ${DARKNET_PATH}/src/local_layer.c
            ${DARKNET_PATH}/src/box.c                     ${DARKNET_PATH}/src/lstm_layer.c
            ${DARKNET_PATH}/src/col2im.c                  ${DARKNET_PATH}/src/matrix.c
            ${DARKNET_PATH}/src/connected_layer.c         ${DARKNET_PATH}/src/maxpool_layer.c
            ${DARKNET_PATH}/src/convolutional_layer.c     ${DARKNET_PATH}/src/network.c
            ${DARKNET_PATH}/src/cost_layer.c              ${DARKNET_PATH}/src/normalization_layer.c
            ${DARKNET_PATH}/src/image_opencv.cpp	      ${DARKNET_PATH}/src/iseg_layer.c
            ${DARKNET_PATH}/src/crnn_layer.c              ${DARKNET_PATH}/src/option_list.c
            ${DARKNET_PATH}/src/crop_layer.c              ${DARKNET_PATH}/src/parser.c
            ${DARKNET_PATH}/src/cuda.c                    ${DARKNET_PATH}/src/region_layer.c
            ${DARKNET_PATH}/src/data.c                    ${DARKNET_PATH}/src/reorg_layer.c
            ${DARKNET_PATH}/src/deconvolutional_layer.c   ${DARKNET_PATH}/src/rnn_layer.c
            ${DARKNET_PATH}/src/demo.c                    ${DARKNET_PATH}/src/route_layer.c
            ${DARKNET_PATH}/src/detection_layer.c         ${DARKNET_PATH}/src/shortcut_layer.c
            ${DARKNET_PATH}/src/dropout_layer.c           ${DARKNET_PATH}/src/softmax_layer.c
            ${DARKNET_PATH}/src/gemm.c                    ${DARKNET_PATH}/src/tree.c
            ${DARKNET_PATH}/src/gru_layer.c               ${DARKNET_PATH}/src/utils.c
            ${DARKNET_PATH}/src/upsample_layer.c          ${DARKNET_PATH}/src/logistic_layer.c
            ${DARKNET_PATH}/src/l2norm_layer.c            ${DARKNET_PATH}/src/yolo_layer.c)
    target_link_libraries(darknet_riley_lib
            m
            pthread
            stdc++
            ${OpenCV_LIBS})
    # Add the yolo executable
    add_executable(yolo
            src/YoloObjectDetector.cpp)
    target_link_libraries(yolo
            darknet_riley_lib)

endif()

# Add the riley executable
add_executable(riley
        ${CMAKE_SOURCE_DIR}/src/riley.cpp)
target_link_libraries(riley
        manipulate_lib
        network_lib)

# Add the manipulation executable
add_executable(manipulate
        ${CMAKE_SOURCE_DIR}/src/manipulate/src/manipulate.cpp)
target_link_libraries(manipulate
        manipulate_lib)
