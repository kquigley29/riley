cmake_minimum_required(VERSION 3.15)

project(riley)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(opencv)
add_subdirectory(opennn)

include_directories(
        opencv
        opennn)