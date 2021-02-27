# Kamera Studio
A simple, minimalist, cross-platform photo editor built in C++ with [wxWidgets](https://www.wxwidgets.org) and [OpenCV](https://opencv.org).

## Table of Contents
1. Requirements
2. Build
3. Run
4. Authors
<hr>

## Requirements
Note that these are **untested suggestions** for achieving the requirements to run this application in a development environment of your choosing.

### Linux Requirements
```sh
$ sudo apt install libgtk-3-dev cmake
```

### Windows Requirements
Install [Visual Studio](https://visualstudio.microsoft.com) and its C++ development environment.

### MacOS Requirements
Install [Homebrew](https://brew.sh).
```sh
$ brew install cmake
```
<hr>


## Build (Linux, Windows, MacOS)
Building will check your system for the wxWidgets and OpenCV dependencies, and install the required dependencies if they are not found. The CMake system that implements the installation of missing dependencies is currently **untested**. If you already have wxWidgets and OpenCV installed, there is nothing to work about.

```sh
$ git clone https://github.com/oscarsandford/kamera.git
$ cd kamera
$ mkdir build
$ cd build
$ cmake ..
$ make
```
<hr>

## Run
```sh
$ cd build
# Recompile and execute. Helpful for testing quick changes.
$ make && subprojects/Build/kamera_core/kamera
```
<hr>

## Authors
Created by Oscar Sandford.