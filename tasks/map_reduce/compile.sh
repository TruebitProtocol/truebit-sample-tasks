#!/bin/sh

rm -rf *.wasm
rm -rf *.js
rm -rf ./build
rm -rf ./dist
rm -rf ./src/main.o
mkdir ./build
mkdir ./dist
cp -rf ./src/* ./build

# Build WASM Base File
cd ./build
emcc -O2 -I $EMSCRIPTEN/system/include -c main.c
emcc -o ../dist/map_reduce.js main.o -lcrypto -lssl
cd ..

# Build / Link For Truebit integration
cd ./dist
node ~/emscripten-module-wrapper/prepare.js map_reduce.js --file input.data --file output.data --run --debug --memory-size=20 --metering=5000 --upload-ipfs --limit-stack
cp ./stacklimit.wasm ../task.wasm
cp ./info.json ../
cd ..

# Build the Solidity Code
cp -f ../../contracts ./build
cp -f ./src/contract.sol ./build
solc --overwrite --bin --abi --optimize ./build/contract.sol -o build
