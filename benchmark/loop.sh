#!/bin/bash

# exit at first error
set -e

RUN_SIZE=10
RUN_DIR='runs'
PREFIX_DIR='run'
TEST_FORMAT='json'
DELAY=1

# help
ARG1=$1
HELP=${ARG1: -2}

if [[ $HELP == "-h" ]]
then
    printf "\tcalls benchmarks in a loop of size $RUN_SIZE\n"
    printf "\teach test run's results are collected into its own directory in '$RUN_DIR' prefixed with '$PREFIX_DIR'\n"
    echo
    exit 0
fi

echo "TEST RUN: ${RUN_SIZE}"
echo

sleep $DELAY

j=0
k=0

for ((i=0; i<$RUN_SIZE; i++))
do
    ((j=i+1))
    k=$(printf "%02d\n" $j)

    echo "run: #${k}"

    DIR_RES="${RUN_DIR}/${PREFIX_DIR}_${k}"
    rm -rf $DIR_RES
    #mkdir -p $DIR_RES

    ./bmark.sh $DIR_RES $TEST_FORMAT
done

echo
echo "++ successfully completed"
