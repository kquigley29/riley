# Root CMake file
cmake_minimum_required(VERSION 3.15)
project(riley)
set(CMAKE_CXX_STANDARD 14)


find_package(OpenCV REQUIRED)

add_subdirectory(src)

add_executable(riley
        src/riley.cpp
        src/data_process/src/VideoProcessor.cpp
        )

target_link_libraries(riley ${OpenCV_LIBS})