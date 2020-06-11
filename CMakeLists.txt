# Root CMake file
cmake_minimum_required(VERSION 3.15)
project(riley)
set(CMAKE_CXX_STANDARD 14)
set(LIBRARY_LINKS lib/data_process_lib lib/darknet_riley_lib)


find_package(OpenCV REQUIRED)

add_subdirectory(src)

add_executable(riley
        src/riley.cpp)

target_link_libraries(riley
        ${OpenCV_LIBS}
        data_process_lib
        darknet_riley_lib)