# Builds riley.exe

g++ src/riley.cpp src/data_process/src/VideoProcessor.cpp -o riley.exe `pkg-config opencv4 --cflags --libs`
