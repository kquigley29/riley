# Builds riley.exe

g++ src/riley.cpp src/data_process/VideoProcessor.cpp -o riley.exe `pkg-config opencv4 --cflags --libs`