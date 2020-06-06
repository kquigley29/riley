cmake_minimum_required(VERSION 3.15)

project(riley)

set(CMAKE_CXX_STANDARD 14)

add_executable(riley riley.cpp)

add_subdirectory(network)
add_subdirectory(video)
add_subdirectory(data)


include_directories(
        network
        video
        data
        opennn
)
