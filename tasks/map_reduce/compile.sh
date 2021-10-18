#!/bin/sh

rm -rf ./build
mkdir ./build

em++ -O2 -I $EMSCRIPTEN/system/include -c -std=c++11 ./src/main.c
em++ -o map_reduce.js main.o -lcrypto -lssl

node ~/emscripten-module-wrapper/prepare.js map_reduce.js --file input.data --file output.data --run --debug --out=dist --memory-size=20 --metering=5000 --upload-ipfs --limit-stack
cp dist/stacklimit.wasm task.wasm
cp dist/info.json .
solc --overwrite --bin --abi --optimize ./src/contract.sol -o build
