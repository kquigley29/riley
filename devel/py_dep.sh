# Install required packages and libraries for python
# And setup the virtual environment

# Install pip3
sudo apt install python3-pip

# Install virtualenv
pip3 install virtualenv

# Check for venv and if it does not exist create it
printf "checking for virtual environment..."
if [ -d venv ]; then
  printf "venv already exists.\n"
else
  printf "Creating venv."
  virtualenv venv
  printf "venv created successfully.\n"
fi

# Source the virtual environment
source venv/bin/activate

# Install the required packages
printf "Installing packages..."
pip3 install numpy scipy scikit-learn sqlalchemy sqlalchemy_utils opencv-python

# Deactivate the virtual environment
deactivate