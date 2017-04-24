#!/bin/bash
rm -rf build
mkdir build
cd build
cmake -G"Ninja" .. && cmake --build . && ctest -V
cd ..
