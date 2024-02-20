#!/bin/bash

echo "Building service..."

cmake -B build -S .

cd build 
make 
cd ..

sudo cp ./build/main /usr/bin/aracli
