#!/bin/bash

HERE=$(realpath $(dirname $0))

if [[ "x$1" == "x" ]]; then
    THERE=$HERE
else
    THERE=$(realpath $1)
fi

SOURCES=$(find $THERE -type f -name "*.cpp" -printf "%P\n")
SOURCES+=$(find $THERE -type f -name "*.h" -printf "%P\n")

for SRC in $SOURCES; do
    FILE=$(basename $SRC)
    mkdir -p $HERE/report/$(dirname $SRC)
    clang-tidy -p=./build/compile_commands.json -checks=google-* $THERE/$SRC &> $HERE/report/$(dirname $SRC)/${FILE}.log 
    echo "report:$(basename $SRC)"
done

echo "done"