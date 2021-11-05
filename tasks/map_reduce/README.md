# MapReduce Example for TrueBit

# Abstract
We provide a template Truebit task for [MapReduce](https://en.wikipedia.org/wiki/MapReduce), a method for processing large data sets.

The following task is intended to be sufficiently abstract to show that developers can plug in their own (not necessarily closed form) formulas.  Suppose we start with a bytes32 array input of pairs with elements of the form <Ethereum address, value> (Ethereum public keys are only 20 bytes, so there is room to create such pairs within 32 bytes).   Consider a task which maps each array element <addr, n> to <addr, n^th prime number>.  Next, filter the resulting array by elements whose "n^th prime number" contains a "3" in the decimal representation, and output this array.

## How To Use

* Follow the build instructions in the top level ReadMe.md file

## Extra Notes

* File: `input.data` is just an example of the format of the input data that would ultimately be attached to the task in the Solidity Contract
