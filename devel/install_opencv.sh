# Clone OpenCV repo for computer vision tools
echo "Checking for opencv..."
if [ -d "src/opencv" ]; then
  echo "opencv exists.\n"
else
  (
    cd src
    echo "opencv does not exist. Cloning from https://github.com/opencv/opencv.git..."
    git clone https://github.com/opencv/opencv.git
    echo "opencv cloned.\n"
  )
fi

# Clone OpenCV repo for computer vision tools
echo "Checking for opencv..."
if [ -d "src/opencv_contrib" ]; then
  echo "opencv_contrib exists.\n"
else
  (
    cd src
    echo "opencv_contrib does not exist. Cloning from https://github.com/opencv/opencv_contrib.git..."
    git clone https://github.com/opencv/opencv_contrib.git
    echo "opencv_contrib cloned.\n"
  )
fi