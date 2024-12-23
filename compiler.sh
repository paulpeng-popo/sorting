#!/bin/sh

# check if only one argument is passed
if [ $# -ne 1 ]; then
    echo "This script compiles the given file using clang"
    echo "Usage: $0 <filename> (c|cc|cpp)"
    echo "Note: Do not support multiple files"
    exit 1
fi

# check if the argument is help
if [ $1 = "help" ]; then
    echo "This script compiles the given file using clang"
    echo "Usage: $0 <filename> (c|cc|cpp)"
    echo "Note: Do not support multiple files"
    exit 0
fi

# check if the file exists
if [ ! -f $1 ]; then
    echo "File not found!"
    exit 1
fi

# determine the file type
# c -> compile using gcc
# cc or cpp -> compile using g++
# others -> print error message

# add -g flag if using gdb/lldb for debugging
flags="-Wall -O2"

case $1 in
    *.c) echo "C file"
        flags="-std=c11 $flags"
        clang $flags -o ${1%.c} $1
        ;;
    *.cc|*.cpp) echo "C++ file"
        flags="-std=c++11 $flags"
        ext=${1##*.}
        clang++ $flags -o ${1%.$ext} $1
        ;;
    *) echo "Unknown file type"
        ;;
esac
