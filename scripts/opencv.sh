# Clone OpenCV repo for computer vision tools
printf "Checking for opencv..."
if [ -d "src/opencv" ]; then
  printf "opencv exists.\n"
else
  (
    cd src || exit
    printf "opencv does not exist. Cloning from https://github.com/opencv/opencv.git..."
    git clone https://github.com/opencv/opencv.git
    printf "opencv cloned.\n"
  )
fi

# Clone OpenCV contributing repo for computer vision tools
printf "Checking for opencv..."
if [ -d "src/opencv_contrib" ]; then
  printf "opencv_contrib exists.\n"
else
  (
    cd src || exit
    printf "opencv_contrib does not exist. Cloning from https://github.com/opencv/opencv_contrib.git..."
    git clone https://github.com/opencv/opencv_contrib.git
    printf "opencv_contrib cloned.\n"
  )
fi