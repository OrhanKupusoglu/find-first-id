#!/bin/bash

# exit at first error
set -e

BMARK_SIZES=(
    1024      #  1 x    1 × 1024
    4096      #  1 x    4 × 1024
    8192      #  1 x    8 × 1024
    16384     #  1 x   16 × 1024
    32768     #  1 x   32 × 1024
    65536     #  1 x   64 x 1024
    131072    #  1 x  128 x 1024
    1048576   #  1 x 1024 x 1024
    8388608   #  8 x 1024 x 1024
    16777216  # 16 x 1024 x 1024
)

# help
ARG1=$1
HELP=${ARG1: -2}

# default arg
DEF_TEST_DIR='runs'
DEF_TEST_FORMAT='json'

if [[ $HELP == "-h" ]]
then
    echo "usage:"
    printf "\t$0 <test_dir> <test_format>\n"
    echo
    printf "\ttest output files in 'test_format' will be saved into 'test_dir'\n"
    echo
    printf "\tdefault test_dir is '$DEF_TEST_DIR', test_dir is created first\n"
    printf "\tdefault test_format is '$DEF_TEST_FORMAT', options are 'json|csv'\n"
    printf "\truns benchmark with the given test sizes:\n"
    printf '\t\t%s\n' "${BMARK_SIZES[@]}"
    echo
    exit 0
fi

# read args
TEST_DIR=${1:-$DEF_TEST_DIR}
TEST_FORMAT=${2:-$DEF_TEST_FORMAT}

LOOP_SIZE=${#BMARK_SIZES[@]}

BMARK_MAX_SIZE=16777216  # 16 x 1024 x 1024
DELAY=1
LINE_SEP=$(printf "%0.s=" {1..80})

echo $LINE_SEP
echo "BENCHMARK SIZES:"
printf '\t%s\n' "${BMARK_SIZES[@]}"
echo $LINE_SEP

sleep $DELAY

mkdir -p $TEST_DIR

j=0

for ((i=0; i<$LOOP_SIZE; i++))
do
    if [[ ${BMARK_SIZES[$i]} -gt ${BMARK_MAX_SIZE} ]]
    then
        echo "** size ${BMARK_SIZES[$i]} > ${BMARK_MAX_SIZE} : ignored"
        continue
    fi

    ((j=i+1))
    ./build.sh ${BMARK_SIZES[$i]}
    echo $LINE_SEP

    if [[ "$TEST_FORMAT" == "csv" ]]
    then
        ./bmark-kupid --benchmark_out_format=csv --benchmark_out=${TEST_DIR}/bmark_${j}.csv
    else
        ./bmark-kupid --benchmark_out_format=json --benchmark_out=${TEST_DIR}/bmark_${j}.json
    fi

    echo $LINE_SEP
done
