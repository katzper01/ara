#!/bin/bash

cmake -B build -S .

cd build 
make 
cd ..

cp ./build/main /usr/bin/aracli
