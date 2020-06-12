# Clone OpenNN repo for network tools
printf "Checking for opennn..."
if [ -d "dep/opennn" ]; then
  printf "opennn exists.\n"
else
  (
    cd src || exit
    printf "opennn does not exist. Cloning from https://github.com/Artelnics/opennn.git..."
    git clone https://github.com/Artelnics/opennn.git
    printf "opennn cloned.\n"
  )
fi