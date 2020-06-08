# Clone OpenNN repo for network tools
echo "Checking for opennn..."
if [ -d "dep/opennn" ]; then
  echo "opennn exists.\n"
else
  (
    cd dep
    echo "opennn does not exist. Cloning from https://github.com/Artelnics/opennn.git..."
    git clone https://github.com/Artelnics/opennn.git
    echo "opennn cloned.\n"
  )
fi