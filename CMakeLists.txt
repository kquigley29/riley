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
set(DARKNET_DIR ${CMAKE_SOURCE_DIR}/darknet)
set(DARKNET_RILEY_DIR ${CMAKE_SOURCE_DIR}/darknet_riley)

add_definitions(-DOPENCV)

# Set all binaries to be placed in riley
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY .)

# Include all the include directories
include_directories(
        ${DARKNET_RILEY_DIR}/include
        ${DARKNET_DIR}/include
        ${DARKNET_DIR}/src)

if (CUDA_FOUND)
    link_directories(/usr/local/cua/lib64)
    # Add the darknet library
    cuda_add_library(darknet_riley
            ${DARKNET_RILEY_DIR}/src/ObjectDetector.cpp
            ${DARKNET_RILEY_DIR}/src/CentroidTracker.cpp
            ${DARKNET_RILEY_DIR}/src/RileyUtils.cpp

            ${DARKNET_DIR}/src/activation_layer.c        ${DARKNET_DIR}/src/im2col.c
            ${DARKNET_DIR}/src/activations.c             ${DARKNET_DIR}/src/image.c
            ${DARKNET_DIR}/src/avgpool_layer.c           ${DARKNET_DIR}/src/layer.c
            ${DARKNET_DIR}/src/batchnorm_layer.c         ${DARKNET_DIR}/src/list.c
            ${DARKNET_DIR}/src/blas.c                    ${DARKNET_DIR}/src/local_layer.c
            ${DARKNET_DIR}/src/box.c                     ${DARKNET_DIR}/src/lstm_layer.c
            ${DARKNET_DIR}/src/col2im.c                  ${DARKNET_DIR}/src/matrix.c
            ${DARKNET_DIR}/src/connected_layer.c         ${DARKNET_DIR}/src/maxpool_layer.c
            ${DARKNET_DIR}/src/convolutional_layer.c     ${DARKNET_DIR}/src/network.c
            ${DARKNET_DIR}/src/cost_layer.c              ${DARKNET_DIR}/src/normalization_layer.c
            ${DARKNET_DIR}/src/image_opencv.cpp	         ${DARKNET_DIR}/src/iseg_layer.c
            ${DARKNET_DIR}/src/crnn_layer.c              ${DARKNET_DIR}/src/option_list.c
            ${DARKNET_DIR}/src/crop_layer.c              ${DARKNET_DIR}/src/parser.c
            ${DARKNET_DIR}/src/cuda.c                    ${DARKNET_DIR}/src/region_layer.c
            ${DARKNET_DIR}/src/data.c                    ${DARKNET_DIR}/src/reorg_layer.c
            ${DARKNET_DIR}/src/deconvolutional_layer.c   ${DARKNET_DIR}/src/rnn_layer.c
            ${DARKNET_DIR}/src/route_layer.c             ${DARKNET_DIR}/src/detection_layer.c
            ${DARKNET_DIR}/src/shortcut_layer.c          ${DARKNET_DIR}/src/dropout_layer.c
            ${DARKNET_DIR}/src/softmax_layer.c           ${DARKNET_DIR}/src/gemm.c
            ${DARKNET_DIR}/src/tree.c                    ${DARKNET_DIR}/src/gru_layer.c
            ${DARKNET_DIR}/src/utils.c                   ${DARKNET_DIR}/src/upsample_layer.c
            ${DARKNET_DIR}/src/logistic_layer.c          ${DARKNET_DIR}/src/l2norm_layer.c
            ${DARKNET_DIR}/src/yolo_layer.c              ${DARKNET_DIR}/src/demo.c

            ${DARKNET_DIR}/src/activation_kernels.cu     ${DARKNET_DIR}/src/crop_layer_kernels.cu
            ${DARKNET_DIR}/src/avgpool_layer_kernels.cu  ${DARKNET_DIR}/src/deconvolutional_kernels.cu
            ${DARKNET_DIR}/src/blas_kernels.cu           ${DARKNET_DIR}/src/dropout_layer_kernels.cu
            ${DARKNET_DIR}/src/col2im_kernels.cu         ${DARKNET_DIR}/src/im2col_kernels.cu
            ${DARKNET_DIR}/src/convolutional_kernels.cu  ${DARKNET_DIR}/src/maxpool_layer_kernels.cu)
    target_link_libraries(darknet_riley
            m
            pthread
            stdc++
            cuda
            cudart
            cublas
            curand
            ${OpenCV_LIBS})

else()

    # Add the darknet_riley library
    add_library(darknet_riley
            ${DARKNET_RILEY_DIR}/src/ObjectDetector.cpp
            ${DARKNET_RILEY_DIR}/src/CentroidTracker.cpp
            ${DARKNET_RILEY_DIR}/src/RileyUtils.cpp

            ${DARKNET_DIR}/src/activation_layer.c        ${DARKNET_DIR}/src/im2col.c
            ${DARKNET_DIR}/src/activations.c             ${DARKNET_DIR}/src/image.c
            ${DARKNET_DIR}/src/avgpool_layer.c           ${DARKNET_DIR}/src/layer.c
            ${DARKNET_DIR}/src/batchnorm_layer.c         ${DARKNET_DIR}/src/list.c
            ${DARKNET_DIR}/src/blas.c                    ${DARKNET_DIR}/src/local_layer.c
            ${DARKNET_DIR}/src/box.c                     ${DARKNET_DIR}/src/lstm_layer.c
            ${DARKNET_DIR}/src/col2im.c                  ${DARKNET_DIR}/src/matrix.c
            ${DARKNET_DIR}/src/connected_layer.c         ${DARKNET_DIR}/src/maxpool_layer.c
            ${DARKNET_DIR}/src/convolutional_layer.c     ${DARKNET_DIR}/src/network.c
            ${DARKNET_DIR}/src/cost_layer.c              ${DARKNET_DIR}/src/normalization_layer.c
            ${DARKNET_DIR}/src/image_opencv.cpp	      ${DARKNET_DIR}/src/iseg_layer.c
            ${DARKNET_DIR}/src/crnn_layer.c              ${DARKNET_DIR}/src/option_list.c
            ${DARKNET_DIR}/src/crop_layer.c              ${DARKNET_DIR}/src/parser.c
            ${DARKNET_DIR}/src/cuda.c                    ${DARKNET_DIR}/src/region_layer.c
            ${DARKNET_DIR}/src/data.c                    ${DARKNET_DIR}/src/reorg_layer.c
            ${DARKNET_DIR}/src/deconvolutional_layer.c   ${DARKNET_DIR}/src/rnn_layer.c
            ${DARKNET_DIR}/src/demo.c                    ${DARKNET_DIR}/src/route_layer.c
            ${DARKNET_DIR}/src/detection_layer.c         ${DARKNET_DIR}/src/shortcut_layer.c
            ${DARKNET_DIR}/src/dropout_layer.c           ${DARKNET_DIR}/src/softmax_layer.c
            ${DARKNET_DIR}/src/gemm.c                    ${DARKNET_DIR}/src/tree.c
            ${DARKNET_DIR}/src/gru_layer.c               ${DARKNET_DIR}/src/utils.c
            ${DARKNET_DIR}/src/upsample_layer.c          ${DARKNET_DIR}/src/logistic_layer.c
            ${DARKNET_DIR}/src/l2norm_layer.c            ${DARKNET_DIR}/src/yolo_layer.c)
    target_link_libraries(darknet_riley
            m
            pthread
            stdc++
            ${OpenCV_LIBS})

endif()

add_executable(riley
        ${DARKNET_RILEY_DIR}/src/Riley.cpp)
target_link_libraries(riley
        darknet_riley)
