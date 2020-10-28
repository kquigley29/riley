# Root CMake file
cmake_minimum_required(VERSION 3.15)
project(riley)
set(CMAKE_CXX_STANDARD 14)

add_definitions(-DRILEY_DEBUG)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

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

set(DARKNET_DIR $ENV{DARKNET})
add_compile_definitions(DARKNET="$ENV{DARKNET}")

find_package(OpenCV REQUIRED)
add_definitions(-DOPENCV)

include_directories(
        include
        ${DARKNET_DIR}/include
        ${DARKNET_DIR}/src
)

if (CUDA_FOUND)
    link_directories(/usr/local/cuda/lib64)
    cuda_add_library(${PROJECT_NAME}
            src/ObjectDetector.cpp
            src/CentroidTracker.cpp
            src/riley_utils.cpp

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
    target_link_libraries(${PROJECT_NAME}
            m
            pthread
            stdc++
            cuda
            cudart
            cublas
            curand
            ${OpenCV_LIBS})

else()

    add_library(${PROJECT_NAME}
            src/ObjectDetector.cpp
            src/CentroidTracker.cpp
            src/riley_utils.cpp

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
            ${DARKNET_DIR}/src/demo.c                    ${DARKNET_DIR}/src/route_layer.c
            ${DARKNET_DIR}/src/detection_layer.c         ${DARKNET_DIR}/src/shortcut_layer.c
            ${DARKNET_DIR}/src/dropout_layer.c           ${DARKNET_DIR}/src/softmax_layer.c
            ${DARKNET_DIR}/src/gemm.c                    ${DARKNET_DIR}/src/tree.c
            ${DARKNET_DIR}/src/gru_layer.c               ${DARKNET_DIR}/src/utils.c
            ${DARKNET_DIR}/src/upsample_layer.c          ${DARKNET_DIR}/src/logistic_layer.c
            ${DARKNET_DIR}/src/l2norm_layer.c            ${DARKNET_DIR}/src/yolo_layer.c)
    target_link_libraries(${PROJECT_NAME}
            m
            pthread
            stdc++
            ${OpenCV_LIBS})

endif()

add_executable(demo
        src/demo.cpp
        )

target_link_libraries(demo
        ${PROJECT_NAME}
        )
