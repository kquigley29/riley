cmake_minimum_required(VERSION 3.15)
project(riley)
set(CMAKE_CXX_STANDARD 14)

set(DARKNET_PATH ../darknet/)
add_definitions(-D DARKNET_FILE_PATH="${DARKNET_PATH}")

include_directories(
        ${OpenCV_INCLUDE_DIRS}
        ${DARKNET_PATH}/src
        ${DARKNET_PATH}/include
        include)

add_library(darknet_riley_lib
        src/YoloObjectDetector.cpp

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
        ${DARKNET_PATH}/src/l2norm_layer.c            ${DARKNET_PATH}/src/yolo_layer.c
        )

target_link_libraries(darknet_riley_lib
        ${OpenCV_LIBS})
