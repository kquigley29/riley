# Riley Roulette Prediction

Riley is written mainly in C++ with the aim of predicting the outcome of roulette. Various open source packages
are used to help create the model and process the data.  

The main file, riley.cpp, is where it all comes together.

### Network
The neural network is created using OpenNN open source software. Further information can be found in network/README.md.

### Video
OpenCV is used to process incoming video which is passed in to the network. Read more at video/README.md.

### Data
The data is stored in video format and in a database as luma values for every pixel in a frame. The file data/README.md
contains more information.
