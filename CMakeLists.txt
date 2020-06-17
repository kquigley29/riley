# Root CMake file
cmake_minimum_required(VERSION 3.15)
project(riley)
set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
set(OpenNN_DIR ${CMAKE_SOURCE_DIR}/src/opennn/opennn)

# Set all binaries to be placed in riley/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Include all the include directories
include_directories(
        ${CMAKE_SOURCE_DIR}/src/manipulate/include
        ${CMAKE_SOURCE_DIR}/src/network/include
        ${OpenNN_DIR})

# Add the manipulate library
add_library(manipulate_lib
        ${CMAKE_SOURCE_DIR}/src/manipulate/src/VideoProcessor.cpp)
target_link_libraries(manipulate_lib
        ${OpenCV_LIBS})

# Add the network library
link_directories(
        ${OpenNN_DIR})
add_subdirectory(${OpenNN_DIR})
add_library(network_lib
        ${CMAKE_SOURCE_DIR}/src/network/src/RileyNet.cpp)
target_link_libraries(network_lib
        opennn)

# Add the riley executable
add_executable(riley.bin
        ${CMAKE_SOURCE_DIR}/src/riley.cpp)
target_link_libraries(riley.bin
        manipulate_lib
        network_lib)





