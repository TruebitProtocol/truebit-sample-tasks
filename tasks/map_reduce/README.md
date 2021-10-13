# MapReduce Example for TrueBit

# Abstract

The practical case to keep in mind here is periodic ecosystem rewards, but I think "map-reduce" has more potential to go viral as a concept.  The following task is intended to be sufficiently abstract to show that developers can plug in their own (not necessarily closed form) formulas.  Suppose we start with a bytes32 array input of pairs with elements of the form <Ethereum address, value> (Ethereum public keys are only 20 bytes, so there is room to create such pairs within 32 bytes).   Consider a task which maps each array element <addr, n> to <addr, n^th prime number>.  Next, filter the resulting array by elements whose "n^th prime number" contains a "3" in the decimal representation, and output this array.  This is simple task for Truebit, while less so for Solidity, and it shows that data processing via Truebit is possible.  To continue the ecosystem rewards analogy, imagine that the addresses which pass through the filter and survive into the final output get some sort of prize.

## How To Use

* Build the library from the root:
    * RUN: `bash ./build`

## Extra Notes

* File: `input.data` is just an example of the format of the input data that would ultimately be attached to the task in the Solidity Contract
