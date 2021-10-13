#!/bin/bash
# Proper header for a Bash script.

rm -rf ./test_build
mkdir ./test_build

cp ./src/*.c ./test_build/
cp ./src/*.h ./test_build/
cp ./test_data/input.data ./test_build/

cd ./test_build
gcc -o run ./main.c
./run
