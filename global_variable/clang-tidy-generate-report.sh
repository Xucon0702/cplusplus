#!/bin/bash

HERE=$(realpath $(dirname $0))

echo "HERE:${HERE}"

if [[ "x$1" == "x" ]]; then
    THERE=$HERE
else
    THERE=$(realpath $1)
fi

SOURCES=$(find $THERE -type f -name "*.cpp" -printf "%P\n")
SOURCES+=$(find $THERE -type f -name "*.h" -printf "%P\n")

echo "SOURCES:${HERE}"

for SRC in $SOURCES; do
    FILE=$(basename $SRC)
    mkdir -p $HERE/report/$(dirname $SRC)
    clang-tidy -p=/mnt/f/study/cplusplus/global_variable/build/compile_commands.json -checks=google-* $THERE/$SRC &> $HERE/report/$(dirname $SRC)/${FILE}.log 
done
