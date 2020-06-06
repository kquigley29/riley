# Installs the files required (opennn and opencv)

echo "Setting up required packages and libraries..."

# Change to riley directory
cd find

# Clone OpenNN repo for network tools
echo "Checking for opennn..."
if [ -d "opennn" ]; then
  echo "opennn exists."
else
  echo "opennn does not exist. Cloning from https://github.com/Artelnics/opennn.git..."
  git clone https://github.com/Artelnics/opennn.git
  echo "opennn cloned."
fi

# Clone OpenCV repo for computer vision tools
echo "Checking for opencv..."
if [ -d "opencv" ]; then
  echo "opencv exists."
else
  echo "opencv does not exist. Cloning from https://github.com/opencv/opencv.git..."
  git clone https://github.com/opencv/opencv.git
  echo "opencv cloned."
fi

