#!/bin/sh

rm -rf *.wasm
rm -rf *.js
rm -rf ./build
rm -rf ./dist
rm -rf ./src/main.o
mkdir ./dist
cp -rf ./src/ ./build/

# Build WASM Base File
cd ./build
emcc -O2 -I $EMSCRIPTEN/system/include -c main.c
emcc -o map_reduce.js main.o -lcrypto -lssl
cp -rf *.js ../dist/
cp -rf *.wasm ../dist/
cd ..

# Build / Link For Truebit integration
cp -rf ./test_data/*.data ./dist/
cd ./dist
node ~/emscripten-module-wrapper/prepare.js map_reduce.js --file input.data --file output.data --run --debug --out=finished --memory-size=20 --metering=5000 --upload-ipfs --limit-stack
cp ./finished/stacklimit.wasm ../task.wasm
cp ./finished/info.json ../
cd ..

# Build the Solidity Code
cp -rf ../../contracts ./build
solc --overwrite --bin --abi --optimize ./build/contract.sol -o build
