cmake_minimum_required(VERSION 3.15)
project(riley)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

option(DEMO OFF)
option(LIBSO OFF)

set(DARKNET $ENV{DARKNET})
add_compile_definitions(DARKNET="$ENV{DARKNET}")

find_package(OpenCV REQUIRED)
find_package(CUDA REQUIRED)

include_directories(include)
include_directories(${DARKNET}/include)
include_directories(${DARKNET}/src)

if(LIBSO)
    add_library(${PROJECT_NAME} SHARED src/ObjectDetector.cpp src/utils.cpp)
else()
    add_library(${PROJECT_NAME} STATIC src/ObjectDetector.cpp src/utils.cpp)
endif()
target_link_libraries(${PROJECT_NAME} ${DARKNET}/libdarknet.so ${OpenCV_LIBS})

if(DEMO)
    add_executable(demo src/demo.cpp)
    target_link_libraries(demo ${PROJECT_NAME})
endif()