cmake_minimum_required(VERSION 3.15)
project(riley)
set(CMAKE_CXX_STANDARD 14)

include_directories(
        include
        src)

add_library(data_process_lib
        src/VideoProcessor.cpp)

target_link_libraries(data_process_lib
        ${OpenCV_LIBS})
