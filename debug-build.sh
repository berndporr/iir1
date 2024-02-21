#!/bin/sh
rm -rf CMakeCache.txt CMakeFiles
cmake . -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
make
