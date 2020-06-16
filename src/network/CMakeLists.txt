cmake_minimum_required(VERSION 3.15)
project(network)
set(CMAKE_CXX_STANDARD 14)

link_directories(
        ${CMAKE_SOURCE_DIR}/../opennn/opennn)
include_directories(
        include
        ${CMAKE_SOURCE_DIR}/../opennn/opennn)

add_library(network_lib
        src/RileyNet.cpp)
target_link_libraries(network_lib
        opennn)

