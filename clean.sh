#!/bin/bash

# exit at first error
set -e

MAIN_EXE=kupid
BITOPS_EXE=bitops
TEST_EXE=test-kupid
BMARK_EXE=bmark-kupid

# display command
set -x

rm -rf benchmark/build
rm -rf benchmark/runs
rm -f benchmark/$BMARK_EXE
rm -f benchmark/bmark_*.json
rm -f benchmark/bmark_*.csv

rm -rf src/build
find ./src -type f -name "$MAIN_EXE" -delete
rm -f rm -f src/src/$BITOPS_EXE

rm -rf test/build
rm -f test/$TEST_EXE

find . -type f -name "*.out" -delete

set +x
echo
echo "++ cleaned"
