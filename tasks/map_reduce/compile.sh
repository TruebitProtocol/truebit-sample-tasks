#!/bin/sh
echo "Cleaning up old build artifacts..."
rm -rf *.wasm
rm -rf *.json
rm -rf ./build
rm -rf ./dist
rm -rf ./src/main.o
mkdir ./dist
cp -rf ./src/ ./build/

# Build WASM Base File
echo "Compiling sample task..."
cd ./build
emcc -O2 -I $EMSCRIPTEN/system/include -c main.c
emcc -o map_reduce.js main.o -lcrypto -lssl
cp -rf *.js ../dist/
cp -rf *.wasm ../dist/
cd ..

# Build / Link For Truebit integration
echo "Building .wasm and .info files..."
cp -rf ./test_data/*.data ./dist/
cd ./dist
node ~/emscripten-module-wrapper/prepare.js map_reduce.js --file input.data --file output.data --run --debug --out=finished --memory-size=20 --metering=5000 --upload-ipfs --limit-stack
cp ./finished/stacklimit.wasm ../task.wasm
cp ./finished/info.json ../
cd ..
echo "Final files are in ./task.wasm and ./info.json"

# Build the Solidity Code
echo "Building smart contract for giving task..."
cp -rf ../../contracts ./build
solc --overwrite --bin --abi --optimize ./build/contract.sol -o build

echo "Build steps complete. Remember to deploy the smart contract with /deploy.sh"
echo "You can test the smart contract with ./send.js"