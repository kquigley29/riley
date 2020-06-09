# Clone Darknet repo for object detection
printf "Checking for darknet..."
if [ -d "src/darknet" ]; then
  printf "darknet exists.\n"
else
  (
    cd src || exit
    printf "darknet does not exist. Cloning from https://github.com/pjreddie/darknet.git..."
    git clone https://github.com/pjreddie/darknet.git
    printf "darknet cloned.\n"
  )
fi