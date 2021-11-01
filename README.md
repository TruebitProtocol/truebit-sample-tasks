# Sample Truebit Task Library

# Abstract

This is a Example Library of Several TrueBit Tasks that are written in different languages and show end-to-end how you would develop and deploy your own TrueBit tasks

## Pre-Reqs to Install

These scripts are designed to be modified, built, and deployed within a running truebit-eth-server Docker image. It is assumed that you already have a properly
configured Truebit installation, and can be connected to an appropriate blockchain for smart contract deployment.

# Quick Start

  * git clone the sample repo into your truebit-docker/wasm_bin directory in native Linux
  * run truebit-eth container with wasm_bin volume, up to and including bash /goerli.sh

```
YYY=$HOME'/truebit-docker'
docker run --network host -v $YYY/docker-clef:/root/.clef \
	-v $YYY/docker-geth:/root/.ethereum \
	-v $YYY/docker-ipfs:/root/.ipfs \
	-v $YYY/wasm_bin:/root/wasm_bin \
	--name truebit --rm -it truebitprotocol/truebit-eth:latest
```

  * open second terminal window into container: docker exec -it truebit /bin/bash
  * cd ; cd wasm_bin/truebit-sample-tasks
  * ./build_env.sh
  * cd tasks/map_reduce
  * sh compile.sh
  * TBD deploy contract (this likely requires npm install somewhere, and running deploy.sh at top level)
  * TBD call with send.js

## List of Example Tasks

* _MapReduce_ Example
    * _Language:_ C
    * _Description:_ Task that takes a list of Binary32 data that are made up of ETH Address (First 20B) and a Value (Last 12B) maps the Value to the n^th prime number, then reduces the list by only looking for Value that contains the number '3'
