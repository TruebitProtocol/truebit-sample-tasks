# Sample Truebit Task Library

# Abstract

This is a Example Library of Several TrueBit Tasks that are written in different languages and show end-to-end how you would develop and deploy your own TrueBit tasks

## Pre-Reqs to Install

These scripts are designed to be modified, built, and deployed within a running truebit-eth-server Docker image. It is assumed that you already have a properly
configured Truebit installation, and can be connected to an appropriate blockchain for smart contract deployment.

# Quick Start

  * git clone the sample repo into your truebit-docker/wasm_bin directory in native Linux

```
YYY=$HOME'/truebit-docker'
docker run --network host -v $YYY/docker-clef:/root/.clef \
	-v $YYY/docker-geth:/root/.ethereum \
	-v $YYY/docker-ipfs:/root/.ipfs \
	-v $YYY/wasm_bin:/root/wasm_bin \
	--name truebit --rm -it truebitprotocol/truebit-eth:latest
```
  * TYPE: ```cd /```
  * TYPE: ```bash ./goerli.sh```
    * Should run a split window with Geth on Top and CLEF on the bottom
    * You need this window to enter in you password for the accounts when prompted
  * open second terminal window into container: ```docker exec -it truebit /bin/bash```
  * TYPE: ```cd ; cd wasm_bin/truebit-sample-tasks```
  * TYPE: ```npm install```
  * TYPE: ```./build_env.sh```
  * TYPE: ```cd tasks/map_reduce```
  * TYPE: ```sh compile.sh```
  * TYPE: ```cd ~/wasm_bin/truebit-sample-tasks```
  * TYPE: ```sh ./deploy.sh <task_name> <account_num>```
    * e.g.  ```sh ./deploy.sh map_reduce 2```
    * Sign the Account password in the running Truebit CLEF
  * TYPE: ```cd tasks/map_reduce```
  * TYPE: ```node ./send.js <account_num> <data_1> <data_2> ...<data_n>```
    * e.g.  ```node ./send.js 2 27dc7AFF9355902358cD000000000021 17dc7AFF9355902358cD000000000401```
    * Sign the Account password in the running Truebit CLEF

## List of Example Tasks

* _MapReduce_ Example
    * _Language:_ C
    * _Description:_ Task that takes a list of Binary32 data that are made up of ETH Address (First 20B) and a Value (Last 12B) maps the Value to the n^th prime number, then reduces the list by only looking for Value that contains the number '3'
