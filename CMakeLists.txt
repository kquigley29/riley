cmake_minimum_required(VERSION 3.15)
project(riley)
set(CMAKE_CXX_STANDARD 14)

option(DEMO OFF)

set(DARKNET $ENV{DARKNET})
add_compile_definitions(DARKNET="$ENV{DARKNET}")

find_package(OpenCV REQUIRED)
find_package(CUDA REQUIRED)

include_directories(include)
include_directories(${DARKNET}/include)
include_directories(${DARKNET}/src)

add_library(${PROJECT_NAME} src/ObjectDetector.cpp src/utils.cpp)
target_link_libraries(${PROJECT_NAME} ${DARKNET}/libdarknet.so ${OpenCV_LIBS})

if(DEMO)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    add_executable(demo src/demo.cpp)
    target_link_libraries(demo ${PROJECT_NAME})
endif()