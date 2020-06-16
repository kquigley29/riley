cmake_minimum_required(VERSION 3.15)
project(manipulate)
set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

include_directories(include)

add_library(manipulate_lib
        src/VideoProcessor.cpp)
target_link_libraries(manipulate_lib
        ${OpenCV_LIBS})
