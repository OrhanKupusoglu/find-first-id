#!/bin/bash

# exit at first error
set -e

# set C++ compiler
source ../src/cxx.sh

TEST_EXE=test-kupid

rm -f $TEST_EXE
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
stat --printf="%n - %s bytes\n" $TEST_EXE

echo
echo "++ list of tests:"
./$TEST_EXE --gtest_list_tests
echo
