#!/bin/bash
# Proper header for a Bash script.

rm *.so
rm *.o
rm ./run

# gcc -c -fPIC ./map/nth_prime.c ./reduce/find_threes.c
# gcc -shared -o lib_nth_prime.so nth_prime.o
# gcc -shared -o lib_find_threes.so find_threes.o
gcc -o run ./main.c