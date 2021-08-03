#!/bin/sh

# TODO: any necessary pre-build steps.

# Final build.
if [ ! -d "build" ]; then 
    mkdir build
fi
cd build 
cmake ..
make
cd ..
