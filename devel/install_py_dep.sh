# Install required packages and libraries for python
# And setup the virtual environment

# Install virtualenv
pip3 install virtualenv

# Check for venv and if it does not exist create it
echo "checking for virtual environment..."
if [ -d venv ]; then
  echo "venv already exists.\n"
else
  echo "Creating venv."
  virtualenv venv
  echo "venv created successfully.\n"
fi

# Source the virtual environment
source venv/bin/activate

# Install the required packages
echo "Installing packages..."
pip3 install numpy scipy scikit-learn sqlalchemy sqlalchemy_utils opencv-python

# Deactivate the virtual environment
deactivate