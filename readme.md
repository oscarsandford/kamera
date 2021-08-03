# Kamera Studio
A simple, minimalist, cross-platform photo editor built in C++ with [wxWidgets](https://www.wxwidgets.org) and [OpenCV](https://opencv.org). **This project is currently being refactored to work with these libraries as submodules.**

## Table of Contents
1. Requirements
2. Build
3. Run
4. Authors
<hr>

## Requirements
Note that these are **untested suggestions** for achieving the requirements to run this application in a development environment of your choosing.

### Linux
```sh
$ sudo apt install libgtk-3-dev cmake
```

### Windows
Install [Visual Studio](https://visualstudio.microsoft.com) and its C++ development environment.

### MacOS
Install [Homebrew](https://brew.sh).
```sh
$ brew install cmake
```
<hr>


## Build (Linux, Windows, MacOS)
Clone the repository with the given argument to download the Git submodules. This may take some time. These libraries will be linked when CMake build is executed.

```sh
$ git clone https://github.com/oscarsandford/kamera.git --recurse-submodules
$ chmod 777 configure.sh
$ ./configure.sh
```
<hr>

Added `external/wxwidgets/include/wx/setup.h` with content copied from `external/wxwidgets/setup.h.in`.


## Authors
Created by Oscar Sandford.