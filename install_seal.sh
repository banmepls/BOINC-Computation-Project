#!/bin/bash
set -e

echo "Updating apt..."
apt-get update

echo "Installing CMake and dos2unix..."
apt-get install -y cmake dos2unix

echo "Checking for SEAL..."
if [ ! -d "SEAL" ]; then
    echo "Cloning SEAL..."
    git clone https://github.com/microsoft/SEAL.git
else
    echo "SEAL directory exists."
fi

cd SEAL
echo "Building SEAL..."
cmake -S . -B build -DSEAL_BUILD_EXAMPLES=OFF
cmake --build build -j$(nproc)
echo "Installing SEAL..."
cmake --install build

echo "SEAL installation complete."
