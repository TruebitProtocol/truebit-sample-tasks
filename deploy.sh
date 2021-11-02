#!/bin/sh

cd tasks/$1
rm -rf ./public
node ../../deploy.js $2
cd ../..
