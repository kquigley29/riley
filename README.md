# Riley
A C++ YOLOv3 object detector library with centroid tracking.

## Purpose
I created this repository as a general objects detection library to use in other projects. 
The use cases for this are vast and therfore I woud like to make this projects as general as possible.

## Build
To build make a build directory make sure you are currently in the riley main directory and run
```bash
mkdir build
cd build
cmake ..
make
cd ..
```
The libraries and example executable should be in the main directory.

## Run 
The example code is a small demonstration of what the code does.  
You will need to have installed the YOLOv3 weights in the weights directory. This can be done by using the commands in weights/get_weights.txt.  
Run the following in your terminal to see it in action:
```bash
./detector yolo
```
This starts the objects detection without tracking on images received from the default camera and with the default configuration files and weights.
Simply run the following to see other input methods:
```bash
./detector -h
```
