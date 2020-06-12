#!/bin/bash

# exit at first error
set -e

# set C++ compiler
source cxx.sh

MAIN_EXE=kupid

rm -f $MAIN_EXE
rm -rf build/

mkdir build
cd build
cmake ../. -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}

echo
make clean
make

cd ..
echo
echo "++ successfully built:"
stat --printf="%n - %s bytes\n" $MAIN_EXE
echo
