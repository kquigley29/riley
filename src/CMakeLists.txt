#cmake_minimum_required(VERSION 3.15)
project(riley)

add_subdirectory(manipulate)
add_subdirectory(network)
add_subdirectory(opennn)

include_directories(
        data_process/include
        network/include)

add_executable(riley
        riley.cpp)
target_link_libraries(riley
        manipulate_lib
        network_lib)