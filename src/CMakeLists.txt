cmake_minimum_required(VERSION 3.15)
project(riley)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(data_process)
add_subdirectory(network)
add_subdirectory(opennn)

include_directories(
        data_process/include
        network/include)
