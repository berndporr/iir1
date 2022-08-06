#!/bin/sh
rm -rf CMakeCache.txt CMakeFiles
cmake . -DCMAKE_BUILD_TYPE=DEBUG
make
