# Prophet 6 VST

Welcome to the Prophet 6 VST project! This guide will walk you through setting up your Mac and building the Prophet 6 VST from scratch, even if you have no programming experience.

## Prerequisites

Before we start, make sure you're using a Mac with the latest macOS installed (currently macOS Monterey or later).

## Step 1: Install Xcode

1. Open the App Store on your Mac.
2. Search for "Xcode" and click "Get" or "Install".
3. Wait for Xcode to download and install (this may take a while).
4. Once installed, open Xcode and agree to the license terms.
5. Xcode will install additional components. Wait for this to complete.

## Step 2: Install Homebrew

Homebrew is a package manager that makes it easy to install developer tools.

1. Open Terminal (you can find it in Applications > Utilities > Terminal).
2. Copy and paste this command into Terminal, then press Enter:
   ```
    "
   ```
3. Follow the prompts on the screen. You may need to enter your Mac's password.
4. After installation, restart Terminal.

## Step 3: Install CMake

1. In Terminal, run this command:
   ```
   brew install cmake
   ```
2. Wait for CMake to install.

## Step 4: Install Git

1. In Terminal, run this command:
   ```
   brew install git
   ```
2. Wait for Git to install.

## Step 5: Download and Install JUCE

1. Go to the [JUCE website](https://juce.com/get-juce/download).
2. Download the latest version of JUCE for macOS.
3. Once downloaded, double-click the .pkg file and follow the installation instructions.

## Step 6: Download the VST3 SDK

1. Go to the [Steinberg VST 3 SDK page](https://www.steinberg.net/developers/).
2. Click on "VST 3 SDK" to download it.
3. Once downloaded, unzip the file.
4. Move the unzipped folder to a location you can easily find, like your Documents folder.

## Step 7: Clone the Prophet 6 VST Repository

1. Open Terminal.
2. Run this command to navigate to your Documents folder:
   ```
   cd ~/Documents
   ```
3. Run this command to clone the repository:
   ```
   git clone https://github.com/SashimiSaketoro/ProphetVST.git
   ```
4. Navigate into the cloned repository:
   ```
   cd Prophet6VST
   ```

## Step 8: Build the Project

1. In Terminal, create a new directory for building:
   ```
   mkdir build
   cd build
   ```
2. Run CMake to configure the project. Replace the paths with the actual locations on your Mac:
   ```
   cmake .. -GXcode -DCMAKE_BUILD_TYPE=Release -DVST3_SDK_ROOT=/path/to/vst3sdk -DJUCE_PATH=/path/to/JUCE
   ```
   For example, if you put the VST3 SDK in your Documents folder and JUCE is installed in the default location, the command might look like this:
   ```
   cmake .. -GXcode -DCMAKE_BUILD_TYPE=Release -DVST3_SDK_ROOT=~/Applications/VST_SDK -DJUCE_PATH="/Applications/JUCE/
   ```
3. Build the project:
   ```
   cmake --build . --config Release
   ```
4. Wait for the build to complete. This may take several minutes.

## Step 9: Locate the Built Files

1. In Finder, navigate to the `build` folder inside your Prophet6VST folder.
2. Look for a folder named `VST3` - this contains your VST3 plugin.
3. Look for a folder named `Standalone` - this contains your standalone application.

## Installing the VST3 Plugin

1. In Finder, go to your home folder.
2. Press Cmd+Shift+. to show hidden files.
3. Navigate to Library > Audio > Plug-Ins > VST3.
4. Drag the .vst3 file from your build folder to this VST3 folder.

## Running the Standalone Application

1. In Finder, navigate to the `Standalone` folder in your build directory.
2. Double-click the Prophet 6 application to run it.

Congratulations! You've successfully built and installed the Prophet 6 VST. You can now use it in any VST3-compatible Digital Audio Workstation or as a standalone application.

If you encounter any issues during this process, don't hesitate to ask for help. Good luck and enjoy your Prophet 6 VST!
