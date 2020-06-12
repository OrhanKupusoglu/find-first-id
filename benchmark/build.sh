#!/bin/bash

# exit at first error
set -e

# set C++ compiler
source ../src/cxx.sh

# default arg
DEF_BMARK_TEST_SIZE=8192
# read arg
BMARK_TEST_SIZE=${1:-$DEF_BMARK_TEST_SIZE}

BMARK_EXE=bmark-kupid

rm -f $BMARK_EXE
rm -rf build/

mkdir build
cd build
export BMARK_TEST_SIZE=${BMARK_TEST_SIZE} && cmake ../. -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}

echo
make clean
make

cd ..
echo
echo "++ successfully built:"
stat --printf="%n - %s bytes\n" $BMARK_EXE

echo
echo "++ test size: $BMARK_TEST_SIZE"
