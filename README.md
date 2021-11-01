# Sample Truebit Task Library

# Abstract

This is a Example Library of Several TrueBit Tasks that are written in different languages and show end-to-end how you would develop and deploy your own TrueBit tasks

## Pre-Reqs to Install

These scripts are designed to be modified, built, and deployed within a running truebit-eth-server Docker image. It is assumed that you already have a properly
configured Truebit installation, and can be connected to an appropriate blockchain for smart contract deployment.

### Install Emscripten

* On A Mac:
    * Instructions Using [HomeBrew](https://formulae.brew.sh/formula/emscripten)

### Build Emscripten Module Wrapper

* On A Mac:
    * From `root` directory:
        * RUN `cd ./emscripten-module-wrapper`
        * RUN `npm install`

## List of Example Tasks

* _MapReduce_ Example
    * _Language:_ C
    * _Description:_ Task that takes a list of Binary32 data that are made up of ETH Address (First 20B) and a Value (Last 12B) maps the Value to the n^th prime number, then reduces the list by only looking for Value that contains the number '3'
