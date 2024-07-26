#!/bin/bash

# Prophet 6 VST Installer

echo "Welcome to the Prophet 6 VST Installer!"
echo "This script will set up your environment and build the Prophet 6 VST."

# Check if Xcode is installed
if ! command -v xcode-select &> /dev/null; then
    echo "Xcode is not installed. Please install Xcode from the App Store and run this script again."
    exit 1
fi

# Install Homebrew if not already installed
if ! command -v brew &> /dev/null; then
    echo "Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
else
    echo "Homebrew is already installed."
fi

# Install CMake
echo "Installing CMake..."
brew install cmake

# Install Git
echo "Installing Git..."
brew install git

# Download and install JUCE
echo "Downloading JUCE..."
brew install juce

# Download VST3 SDK
echo "Downloading VST3 SDK..."
VST3_SDK_DIR="$HOME/VST_SDK"
mkdir -p "$VST3_SDK_DIR"
curl -L "https://download.steinberg.net/sdk_downloads/vst-sdk_3.7.6_build-17_2022-04-29.zip" -o "$VST3_SDK_DIR/vst_sdk.zip"
unzip "$VST3_SDK_DIR/vst_sdk.zip" -d "$VST3_SDK_DIR"
rm "$VST3_SDK_DIR/vst_sdk.zip"

# Clone the Prophet 6 VST repository
echo "Cloning Prophet 6 VST repository..."
git clone https://github.com/SashimiSaketoro/ProphetVST.git
cd Prophet6VST

# Build the project
echo "Building the Prophet 6 VST..."
mkdir build
cd build
cmake .. -GXcode -DCMAKE_BUILD_TYPE=Release -DVST3_SDK_ROOT="$VST3_SDK_DIR/VST_SDK" -DJUCE_PATH="/usr/local/opt/juce"
cmake --build . --config Release

# Install the VST3 plugin
echo "Installing the VST3 plugin..."
mkdir -p "$HOME/Library/Audio/Plug-Ins/VST3"
cp -R "VST3/Prophet6.vst3" "$HOME/Library/Audio/Plug-Ins/VST3/"

# Create a symbolic link for the standalone app
echo "Creating a symbolic link for the standalone app..."
ln -s "$PWD/Standalone/Prophet6.app" "/Applications/Prophet6.app"

echo "Installation complete!"
echo "You can find the VST3 plugin in $HOME/Library/Audio/Plug-Ins/VST3/"
echo "The standalone app is available in your Applications folder."
