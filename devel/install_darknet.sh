# Clone Darknet repo for object detection
echo "Checking for darknet..."
if [ -d "src/darknet" ]; then
  echo "darknet exists.\n"
else
  (
    cd src
    echo "darknet does not exist. Cloning from https://github.com/pjreddie/darknet.git..."
    git clone https://github.com/pjreddie/darknet.git
    echo "darknet cloned.\n"
  )
fi